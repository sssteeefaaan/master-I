# Generated by Django 4.0.2 on 2023-01-24 13:35

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('students', '0005_alter_student_image'),
    ]

    operations = [
        migrations.AlterField(
            model_name='student',
            name='password',
            field=models.CharField(max_length=256),
        ),
    ]