from django import forms

from meetups.models import Participant

class RegistrationForm(forms.ModelForm):
    class Meta:
        model = Participant
        fields = ["email"]