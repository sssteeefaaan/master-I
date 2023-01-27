ARG DOCKER_IMAGE_VERSION

FROM python:${DOCKER_IMAGE_VERSION} as build

WORKDIR /app

ENV PYTHONDONTWRITEBYTECODE 1
ENV PYTHONUNBUFFERED 1

RUN apt-get update && \
    apt-get install -y --no-install-recommends gcc

RUN python -m venv /opt/venv

ENV PATH="/opt/venv/bin:$PATH"

COPY ./app/requirements.txt .

RUN pip install -r requirements.txt --no-cache-dir

COPY ./app /app

RUN chmod +x ./start.sh


# Final
FROM python:${DOCKER_IMAGE_VERSION}

COPY --from=build /opt/venv /opt/venv

WORKDIR /app

ENV PATH="/opt/venv/bin:$PATH"

CMD ./start.sh