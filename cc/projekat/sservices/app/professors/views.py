from django.shortcuts import render, redirect
# Create your views here.

from .forms import ProfessorInfoForm
from .models import Professor
from home.utilities import get_or_create_default_faculty
from django.contrib.auth.hashers import make_password
import logging
logging.basicConfig(level=logging.NOTSET)
from django.db import transaction

from home.forms import AddressForm
from home.models import Address
from requests import post
from os import environ

PROFESSOR_REGISTER_URL = environ.get("PROFESSOR_REGISTER_URL", "PROFESSOR_REGISTER_URL ENV NOT SET")

def register(request):
    try:
        errors = []
        if request.method == "GET":
            professorForm = ProfessorInfoForm(prefix="professor-info")
            professorAddressForm = AddressForm(prefix="professor-address")
        elif request.method == "POST":
            professorForm = ProfessorInfoForm(request.POST, files=request.FILES, prefix="professor-info")
            professorAddressForm = AddressForm(request.POST, prefix="professor-address")
            f1 = professorAddressForm.is_valid()
            f2 = professorForm.is_valid()
            if f1 and f2:
                addressData = professorAddressForm.cleaned_data
                try:
                    with transaction.atomic():
                        professorAddress, _ = Address.objects.get_or_create(**addressData)
                        professorData = professorForm.cleaned_data | dict(address=professorAddress)
                        professorData['password'] = make_password(professorData['password'])
                        professor, _ = Professor.objects.get_or_create(**professorData)
                        faculty = get_or_create_default_faculty()
                        if faculty not in professor.faculties.all():
                            professor.faculties.add(faculty)
                        res = post(PROFESSOR_REGISTER_URL, json=dict(
                            sservice=environ.get("FACULTY_CODE", "ENV FACULTY CODE NOT SET"),
                            professor=professorData | dict(
                                image=professor.image.path,
                                address= addressData
                            )))
                        if res.status_code == 200:
                            return redirect('professor-profile', username=professor.username)
                        else:
                            errors = res.json()["errors"]
                            if type(errors) is str:
                                errors = [errors,]
                            raise Exception(errors)
                except Exception as e:
                    logging.debug(e)
        else:
            raise Exception(f"[Professor-Register]: Unknown method {request.method}!")
        return render(request, 'professors/register.html', {
            'professorForm': professorForm,
            'professorAddressForm': professorAddressForm,
            'errors': errors
        })
    except Exception as e:
        logging.debug(e)
        return redirect('error-page')

def profile(request, username):
    try:
        professor = Professor.objects.get(username=username)
        if professor:
            return render(request, 'professors/profile.html', {
                'professor': professor,
                'professor_dict': professor.as_data()
            })
        raise Exception("Professor not found!")
    except Exception as e:
        logging.debug(e)
        return redirect('error-page')