# Generated by Django 4.0.2 on 2023-01-25 21:38

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('students', '0008_alter_student_image'),
    ]

    operations = [
        migrations.AlterField(
            model_name='student',
            name='image',
            field=models.ImageField(default='images/students/default.jpg', upload_to='images/students'),
        ),
    ]
