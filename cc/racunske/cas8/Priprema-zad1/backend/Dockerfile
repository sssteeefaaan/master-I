FROM node

WORKDIR /app

COPY package.json .

RUN npm install

COPY . .

ENV MONGO_CONNECTION_STRING="mongodb://mongodb:27017/course-goals"

ENV PORT=80

EXPOSE 80

CMD npm start

