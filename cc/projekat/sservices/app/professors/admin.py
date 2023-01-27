from django.contrib import admin

# Register your models here.
from .models import Professor

class ProfessorAdmin(admin.ModelAdmin):
    list_display=('umcn', 'idCardNumber', 'lastName', 'firstName')
    list_filter=('faculties', )

admin.site.register(Professor, ProfessorAdmin)