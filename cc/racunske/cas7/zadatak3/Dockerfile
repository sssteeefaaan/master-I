FROM python

WORKDIR /app

COPY . /app

RUN pip install --upgrade pip

RUN pip install -r req.txt

CMD python manage.py makemigrations && python manage.py migrate && python manage.py runserver 0.0.0.0:8000