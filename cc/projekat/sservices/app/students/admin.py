from django.contrib import admin

from .models import Student, Parent

class StudentAdmin(admin.ModelAdmin):
  list_display = ('index', 'umcn', 'idCardNumber', 'email', 'lastName', 'firstName')
  list_filter = ('address', 'parent')
  #prepopulated_fields = {'slug': ('title', )}

class ParentAdmin(admin.ModelAdmin):
  list_display = ('umcn', 'idCardNumber', 'lastName', 'firstName')
  list_filter = ('address', )


admin.site.register(Student, StudentAdmin)
admin.site.register(Parent, ParentAdmin)