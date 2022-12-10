FROM node

WORKDIR /app

COPY ./package.josn /app/package.json

RUN npm install

COPY ./app.js /app/app.js

COPY ./models /app/models

ENV PORT=80
ENV DB_DB=coarse-goals
ENV DB_IP=mongodb-container
ENV DB_PORT=27017

CMD node app.js