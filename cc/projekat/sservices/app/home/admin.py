from django.contrib import admin

# Register your models here.

from .models import Address, Faculty

class AddressAdmin(admin.ModelAdmin):
  list_display = ('postalCode', 'municipality')
  list_filter = ('postalCode', 'municipality')

class FacultyAdmin(admin.ModelAdmin):
  list_display = ('code', 'address')
  list_filter = ('address', )

admin.site.register(Address, AddressAdmin)
admin.site.register(Faculty, FacultyAdmin)