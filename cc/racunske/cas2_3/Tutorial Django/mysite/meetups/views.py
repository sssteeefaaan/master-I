from django.http import HttpResponse, HttpRequest
from django.shortcuts import render, redirect

from meetups.models import Meetup, Participant
from meetups.forms import RegistrationForm

# Create your views here.
def index(request):
    meetups = Meetup.objects.all()
    # meetups = [
    #     {'title':'A First Meetup', 'location':'New York', 'slug': 'a-first-meetup'},
    #     {'title':'A Second Meetup', 'location':'Paris','slug':'a-second-meetup'}
    # ]
    return render(request, 'meetups/index.html',{
        'meetups': meetups,
        'show_meetups':True
    } )

def meetup_details(request : HttpRequest, meetup_slug):
    try:
        selected_meetup = Meetup.objects.get(slug=meetup_slug)
        # selected_meetup={
        #     'title':'First Meetup',
        #     'description':'first meetup...'
        # }
        if request.method == "GET":
            registration_form = RegistrationForm()
        else:
            registration_form = RegistrationForm(request.POST)

            if registration_form.is_valid():
                user_email = registration_form.cleaned_data["email"]
                participant, _ = Participant.objects.get_or_create(email=user_email)
                selected_meetup.participants.add(participant)
                return redirect("confirm_registration.html")
            

        return render(request, 'meetups/meetup-details.html', {
            'meetup': selected_meetup,
            'meetup_found': True,
            'form': registration_form
        })
    except Exception as e:
        return render(request, 'meetups/meetup-details.html',{
            'meetup_found': False
        })

def confirm_registration(request : HttpRequest):
    return render(request, "meetups/registration_success.html")

