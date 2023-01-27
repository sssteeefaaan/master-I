from django.urls import path, re_path
from django.views.generic.base import RedirectView

from . import views

urlpatterns = [
  path('', RedirectView.as_view(url='/home/')),
  path('home/', view=views.home, name='home-page'),
  path('home/<nav>', view=views.home, name='home-page'),
  path('error/', views.error, name='error-page'),
  re_path('.', RedirectView.as_view(url='/error/'))
]