package com.example.kafka_topic_producer;

public interface KafkaConstants {
    public static String KAFKA_BROKERS = System.getenv("KAFKA_BROKERS");

    public static String CLIENT_ID = "producer1";

    public static String TOPIC_NAME = System.getenv("TOPIC_NAME");
}
