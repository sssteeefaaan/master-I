from django.db import models
from django.core.serializers import serialize

# Create your models here.
class Professor(models.Model):
    firstName = models.CharField(max_length=20, null=False)
    lastName = models.CharField(max_length=20, null=False)
    username = models.CharField(max_length=20, null=False, unique=True, db_index=True)
    password = models.CharField(max_length=256, null=False)
    email = models.CharField(max_length=50, null=False, unique=True, db_index=True)
    umcn = models.CharField(max_length=13, null=False, db_index=True, unique=True)
    idCardNumber = models.CharField(max_length=14,null=False, db_index=True, unique=True)
    phoneNumber = models.CharField(max_length=14, null=False, unique=True, db_index=True)
    address = models.ForeignKey('home.Address', on_delete=models.CASCADE, null=False)
    faculties = models.ManyToManyField('home.Faculty')
    image = models.ImageField(upload_to="images/professors", default='images/professors/default.jpg')

    def __str__(self) -> str:
        return f"{self.umcn} {self.lastName} {self.firstName}"

    def as_data(self) -> dict:
        return {
            "First Name": self.firstName,
            "Last Name": self.lastName,
            "Phone Number": self.phoneNumber,
            "Email": self.email,
            "Username": self.username,
            "Password": self.password,
            "Unique Master Citizen Number": self.umcn,
            "ID Card Number": self.idCardNumber,
            "Address": self.address.as_data(),
            "Faculties": [x.as_data() for x in self.faculties.all()]
        }