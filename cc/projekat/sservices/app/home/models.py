from django.db import models
from django.core.serializers import serialize

# Create your models here.
class Address(models.Model):
    postalCode = models.IntegerField()
    municipality = models.CharField(max_length=100)
    name = models.CharField(max_length=200)

    def __str__(self) -> str:
        return f"{self.postalCode} {self.municipality} {self.name}"

    def as_data(self) -> dict:
        return {
            "Postal Code": self.postalCode,
            "Name": self.name,
            "Municipality": self.municipality,
        }

    def as_dict(self) -> dict:
        ret = dict()
        for (k, v) in self.__dict__.items():
            if isinstance(v, models.Model):
                ret[k] = v.as_dict()
            else:
                ret[k] = str(v)
        return ret

class Faculty(models.Model):
    code = models.CharField(max_length=10, null=False, db_index=True, unique=True)
    name = models.CharField(max_length=50, null=False)
    address = models.ForeignKey(Address, on_delete=models.CASCADE, null=False)
    phoneNumber = models.CharField(max_length=20, null=False, db_index=True, unique=True)

    def __str__(self) -> str:
        return f"{self.code} - {self.name}"

    def as_data(self) -> dict:
        return {
            "Name": self.name,
            "Phone Number": self.phoneNumber,
            "Address": self.address.as_data(),
        }
