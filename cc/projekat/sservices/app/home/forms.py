from django import forms

from .models import Address, Faculty

class AddressForm(forms.ModelForm):
    class Meta:
        model = Address
        exclude = []
        labels = dict(
            postalCode="Postal Code"
        )
        widgets = dict(
            postalCode=forms.NumberInput(attrs=dict(
                placeholder="Input Postal Code"
            )),
            municipality = forms.TextInput(attrs=dict(
                placeholder="Input Municipality"
            )),
            name = forms.TextInput(attrs=dict(
                placeholder="Input Address Name"
            ))
        )

class FacultyForm(forms.ModelForm):
    class Meta:
        model = Faculty
        exclude = ['address']
        labels = dict(
            code = "Faculty Code"
        )
        widgets = dict(
            code=forms.TextInput(attrs=dict(
                placeholder="Input Faculty Code"
            )),
            name = forms.TextInput(attrs=dict(
                placeholder="Input Faculty Name"
            ))
        )