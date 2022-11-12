from django.contrib import admin

# Register your models here.
from .models import Meetup, Location, Participant

class LocationAdmin(admin.ModelAdmin):
    list_display = ('name', 'address')

admin.site.register(Meetup)
admin.site.register(Location, LocationAdmin)
admin.site.register(Participant)