
from django.urls import path
from . import views

urlpatterns = [
    path('', views.index),
    path('success', views.confirm_registration, name="confirm_registration"),
    path('<slug:meetup_slug>', views.meetup_details, name='meetup-details')
]