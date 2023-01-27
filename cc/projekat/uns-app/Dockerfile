ARG DOCKER_IMAGE_VERSION

FROM node:${DOCKER_IMAGE_VERSION} as build

WORKDIR /app

COPY ./app/package.json /app

RUN npm install --no-cache-dir

COPY ./app /app

RUN chown -R node:node /app

USER node

CMD npm start