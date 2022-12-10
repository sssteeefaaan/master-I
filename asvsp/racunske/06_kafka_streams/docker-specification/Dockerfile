FROM maven AS maven_build

COPY ./producer /usr/producer
RUN mvn -f /usr/producer/pom.xml clean package

COPY ./primeri /usr/consumers
RUN mvn -f /usr/consumers/pom.xml clean package

FROM openjdk:8
COPY --from=maven_build /usr/producer/target /usr/producer
COPY --from=maven_build /usr/consumers/target /usr/consumers
ENTRYPOINT ["java","-cp","/usr/producer/kafka-topic-producer-0.0.1-SNAPSHOT.jar", "com.example.kafka_topic_producer.App"]  