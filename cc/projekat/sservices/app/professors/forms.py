from django import forms

from .models import Professor
class ProfessorInfoForm(forms.ModelForm):
    class Meta:
        model = Professor
        exclude = ['faculties', 'address']
        labels = dict(
            firstName="First Name",
            lastName="Last Name",
            phoneNumber="Phone Number",
            email="Email Address",
            umcn = "Unique Master Citizen Number",
            idCardNumber = "Identification Card Number",
            birthDate = "Birth Date",
            image = "Profile Image"
        )
        widgets = dict(
            firstName=forms.TextInput(attrs=dict(
                placeholder="Input First Name"
            )),
            lastName = forms.TextInput(attrs=dict(
                placeholder="Input Last Name"
            )),
            phoneNumber = forms.NumberInput(attrs=dict(
                placeholder="Input Phone Number"
            )),
            email = forms.EmailInput(attrs=dict(
                placeholder="Input Email Address"
            )),
            username = forms.TextInput(attrs=dict(
                placeholder="Input Username"
            )),
            password = forms.PasswordInput(attrs=dict(
                placeholder="Input Password"
            )),
            umcn = forms.NumberInput(attrs=dict(
                placeholder="Input Unique Master Citizen Number"
            )),
            idCardNumber = forms.NumberInput(attrs=dict(
                placeholder="Input Identification Card Number"
            )),
            birthDate = forms.DateInput(attrs=dict(
                placeholder="Input Birth Date"
            )),
            image = forms.FileInput(attrs=dict(
                placeholder="Input Profile Image",
                initial="default.jpg"
            ))
        )