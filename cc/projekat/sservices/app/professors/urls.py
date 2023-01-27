from django.urls import path
from django.views.generic.base import RedirectView

from .views import register, profile

urlpatterns = [
  path('register/', view=register, name='professor-register'),
  path('profile/<username>', view=profile, name='professor-profile'),
  path('', RedirectView.as_view(url='/professors/register/'))
]