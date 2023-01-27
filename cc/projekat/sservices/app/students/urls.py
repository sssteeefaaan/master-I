from django.urls import path
from django.views.generic.base import RedirectView

from . import views

urlpatterns = [
    path('register/', view=views.register, name='student-register'),
    path('profile/<username>', view=views.profile, name='student-profile'),
    path('', RedirectView.as_view(url="/students/register"))
]
