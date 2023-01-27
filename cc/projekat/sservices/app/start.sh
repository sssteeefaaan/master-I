#!/bin/bash

FILE="$DJANGO_ADMINS_FILE_PATH"

python3 manage.py makemigrations
python3 manage.py migrate
if [ ! -f "$FILE" ] || [ $(cat "$FILE" | grep -c "$DJANGO_SUPERUSER_USERNAME") -eq 0 ] ; then
  python3 manage.py createsuperuser --noinput
  echo "$DJANGO_SUPERUSER_USERNAME;$DJANGO_SUPERUSER_PASSWORD;$DJANGO_SUPERUSER_EMAIL" >> "$FILE"
fi
python3 manage.py runserver "$SERVER_PORT"