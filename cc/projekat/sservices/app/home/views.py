from django.shortcuts import render, redirect

from students.models import Student
from professors.models import Professor
from os import environ

# Create your views here.
def home(request, nav = '0'):
    if nav == '0':
        data = dict(
            nav='0',
            items=Professor.objects.all(),
            chosen="Professors",
            profile_link='professor-profile'
        )
    elif nav == '1':
        data = dict(
            nav='1',
            items = Student.objects.all(),
            chosen="Students",
            profile_link='student-profile'
        )
    else:
        return redirect('error-page')
    return render(request, 'home/index.html', data | dict(faculty_name=environ.get("FACULTY_NAME", "FACULTY_NAME ENV NOT SET!"), uns_portal=environ.get("UNS_PORTAL_URL", "UNS_PORTAL_URL ENV NOT SET!")))

def error(request):
    return render(request, 'home/error.html')