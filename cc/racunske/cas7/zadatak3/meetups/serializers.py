from rest_framework import serializers

from meetups.models import Location



class LocationSerializer(serializers.ModelSerializer):
    class Meta:
        model = Location
        fields = ['name','address']
