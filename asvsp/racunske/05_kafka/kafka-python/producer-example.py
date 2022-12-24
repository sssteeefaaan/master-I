from kafka import KafkaProducer
from sys import argv
import json

FILE = argv[1] if len(argv) > 1 else './prepared_messages.txt'
TOPIC = argv[2] if len(argv) > 2 else 'python_topic'

PRODUCER_CONFIG = {
    "bootstrap_servers": argv[3] if len(argv) > 3 else "localhost:9092",
    "max_request_size" : int(argv[4]) if len(argv) > 4 else 100000,
    "key_serializer": str.encode,
    "value_serializer": lambda x: json.dumps(x).encode('utf-8')
}

if __name__ == '__main__':

    producer = KafkaProducer(**PRODUCER_CONFIG)

    def hash(key):
        return len(key)-1

    with open(FILE) as f:
        lines = f.readlines()

        for l in lines:
            line_split = l.split(':')
            record_key = line_split[0]
            record_value = line_split[1]

            print("Producing record: {}\t{}".format(record_key, record_value))
            producer.send(TOPIC, key=record_key, value=record_value)#, partition=hash(record_key))

        producer.flush()

