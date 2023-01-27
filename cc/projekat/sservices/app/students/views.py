from django.shortcuts import render, redirect
# Create your views here.

from .forms import StudentInfoForm, ParentInfoForm
from .models import Student, Parent
from home.utilities import get_or_create_default_faculty
from django.contrib.auth.hashers import make_password
from django.db import transaction
import logging
from os import environ
logging.basicConfig(level=logging.NOTSET)

from home.forms import AddressForm
from home.models import Address

from requests import post

STUDENT_REGISTER_URL=environ.get("STUDENT_REGISTER_URL", "STUDENT_REGISTER_URL ENV NOT SET")

def register(request):
    try:
        errors = []
        if request.method == "GET":
            studentForm = StudentInfoForm(prefix="student-info")
            studentAddressForm = AddressForm(prefix="student-address")
            parentForm = ParentInfoForm(prefix="parent-info")
            parentAddressForm = AddressForm(prefix="parent-address")
        elif request.method == "POST":
            studentForm = StudentInfoForm(request.POST, files=request.FILES, prefix="student-info")
            studentAddressForm = AddressForm(request.POST, prefix="student-address")
            parentForm = ParentInfoForm(request.POST, prefix="parent-info")
            parentAddressForm = AddressForm(request.POST, prefix="parent-address")
            f1 = studentAddressForm.is_valid()
            f2 = parentAddressForm.is_valid()
            f3 = parentForm.is_valid()
            f4 = studentForm.is_valid()
            # Short-circuit-ing gotta make it ugly
            if f1 and f2 and f3 and f4:
                try:
                    with transaction.atomic():
                        studentAddressData = studentAddressForm.cleaned_data
                        studentAddress, _ = Address.objects.get_or_create(**studentAddressData)
                        parentAddressData = parentAddressForm.cleaned_data
                        parentAddress, _ = Address.objects.get_or_create(**parentAddressData)
                        parentData = parentForm.cleaned_data | dict(address=parentAddress)
                        parent, _ = Parent.objects.get_or_create(**parentData)
                        studentData = studentForm.cleaned_data | dict(faculty=get_or_create_default_faculty(), address=studentAddress, parent=parent)
                        studentData['password'] = make_password(studentData['password'])
                        student = Student.objects.create(**studentData)
                        res = post(STUDENT_REGISTER_URL, json=dict(
                            sservice=environ.get("FACULTY_CODE", "ENV FACULTY CODE NOT SET"),
                            student=studentData | dict(
                                birthDate=student.birthDate.isoformat(),
                                image=student.image.path,
                                address=studentAddressData,
                                parent = parentData | dict(
                                    address=parentAddressData
                                ),
                                faculty=environ.get("FACULTY_CODE", "ENV FACULTY CODE NOT SET")
                            )))
                        if res.status_code == 200:
                            return redirect('student-profile', username=student.username)
                        else:
                            errors = res.json()["errors"]
                            if type(errors) is str:
                                errors = [errors,]
                            raise Exception(errors)
                except Exception as e:
                    logging.debug(e)
        else:
            raise Exception(f"[Student-Register]: Unknown method {request.method}!")
        return render(request, 'students/register.html', {
            'studentForm': studentForm,
            'studentAddressForm': studentAddressForm,
            'parentForm': parentForm,
            'parentAddressForm': parentAddressForm,
            'errors': errors
        })
    except Exception as e:
        logging.debug(e)
        return redirect('error-page')

def profile(request, username):
    try:
        student = Student.objects.get(username=username)
        if student:
            return render(request, 'students/profile.html', {
                'student': student,
                'student_dict': student.as_data()
            })
        raise Exception("Student not found!")
    except Exception as e:
        logging.debug(e)
        return redirect('error-page')