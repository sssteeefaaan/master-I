#!/usr/bin/python3

from json import dumps
from os import environ
import requests
import time

from kafka import KafkaProducer


DATASET_API_LINK = environ.get("DATASET_API_LINK", None)
KAFKA_CONFIGURATION = {
    "bootstrap_servers": environ.get("KAFKA_BROKER", "localhost:29092").split(","),
    "key_serializer": lambda x: str.encode("" if not x else x, encoding='utf-8'),
    "value_serializer": lambda x: dumps(dict() if not x else x).encode(encoding='utf-8'),
    "reconnect_backoff_ms": int(environ.get("KAFKA_RECONNECT_BACKOFF_MS", "100"))
}
KAFKA_TOPIC = environ.get("KAFKA_TOPIC", "default")


def send_to_kafka(data):
    try:
        connecting = True
        while connecting:
            try:
                producer = KafkaProducer(**KAFKA_CONFIGURATION)
                if producer.bootstrap_connected():
                    connecting = False
            except Exception as e:
                print(f"[Kafka-Producer-Connecting-Error]: {e}")
            print("Retrying...")
        print(f"Connected to { KAFKA_CONFIGURATION['bootstrap_servers'] }!")
        for segment in data:
            producer.send(topic=KAFKA_TOPIC, key=segment.get('Commodity', None), value=segment)
            print(f"Sent {segment}")
            time.sleep(2)
        producer.flush()
    except Exception as e:
        print(f"[Kafka-Error]: { e }")

def main():
    try:
        print(f"DATASET_API_LINK={ DATASET_API_LINK }")
        print(f"KAFKA_TOPIC={ KAFKA_TOPIC }")
        print(f"KAFKA_CONFIGURATION={ KAFKA_CONFIGURATION }")
        response = requests.get(url=DATASET_API_LINK)
        if response.ok:
            data = response.json()
            print("Got data")#data)
            send_to_kafka(data)
        else:
            raise Exception(f"Response status code: { response.status_code }\nResponse text: { response.text }")
    except Exception as e:
        print(f"[Error]: { e }")

if __name__ == "__main__":
    main()