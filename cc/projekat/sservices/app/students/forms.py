from django import forms

from .models import Student, Parent

class StudentInfoForm(forms.ModelForm):
    class Meta:
        model = Student
        exclude = ['faculty', 'address', 'parent']
        labels = dict(
            firstName="First Name",
            lastName="Last Name",
            phoneNumber="Phone Number",
            email="Email Address",
            umcn = "Unique Master Citizen Number",
            idCardNumber = "Identification Card Number",
            index = "Index Number",
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
            index = forms.TextInput(attrs=dict(
                placeholder="Input Index Number"
            )),
            birthDate = forms.DateInput(attrs=dict(
                placeholder="Input Birth Date"
            )),
            image = forms.FileInput(attrs=dict(
                placeholder="Input Profile Image",
                initial="default.jpg"
            ))
        )

class ParentInfoForm(forms.ModelForm):
    class Meta:
        model = Parent
        exclude = ['address']
        labels = dict(
            firstName = "First Name",
            lastName = "Last Name",
            umcn = "Unique Master Citizen Number",
            idCardNumber = "Identification Card Number",
            phoneNumber = "Phone Number"
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
            umcn = forms.NumberInput(attrs=dict(
                placeholder="Input Unique Master Citizen Number"
            )),
            idCardNumber = forms.NumberInput(attrs=dict(
                placeholder="Input Identification Card Number"
            ))
        )