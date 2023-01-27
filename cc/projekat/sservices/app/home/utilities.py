from .models import Address, Faculty
from os import environ

def get_or_create_address(values):
    a, _ = Address.objects.get_or_create(**values)
    return a

def get_or_create_faculty(values):
    f, _ = Faculty.objects.get_or_create(**values)
    return f

def get_or_create_default_faculty():
    return get_or_create_faculty(dict(
        code=environ.get("FACULTY_CODE", "FACULTY_CODE ENV NOT SET"),
        name=environ.get("FACULTY_NAME", "FACULTY_NAME ENV NOT SET"),
        phoneNumber=environ.get("FACULTY_PHONE_NUMBER", "FACULTY_PHONE_NUMBER ENV NOT SET"),
        address=get_or_create_address(dict(
            postalCode=environ.get("FACULTY_ADDRESS_POSTAL_CODE", "FACULTY_ADDRESS_POSTAL_CODE ENV NOT SET"),
            name=environ.get("FACULTY_ADDRESS_NAME", "FACULTY_ADDRESS_NAME ENV NOT SET"),
            municipality=environ.get("FACULTY_ADDRESS_MUNICIPALITY", "FACULTY_ADDRESS_MUNICIPALITY ENV NOT SET")
        ))
    ))