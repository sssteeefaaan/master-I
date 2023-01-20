FROM python:${PYTHON_PRODUCER_VERSION:-3.7-slim}

WORKDIR /app

COPY . .

RUN pip install --no-cache-dir --upgrade pip

RUN pip install kafka-python requests

CMD python3 ./main.py