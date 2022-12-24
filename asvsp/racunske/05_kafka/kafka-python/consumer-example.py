from sys import argv
from json import loads

from kafka import KafkaConsumer

TOPIC = argv[1] if len(argv) > 1 else 'python_topic'
TIMEOUT = int(argv[2]) if len(argv) > 2 else 1000
MAX_RECORDS = int(argv[3]) if len(argv) > 3 else 10

CONSUMER_CONFIG = {
    "bootstrap_servers": argv[4] if len(argv) > 4 else "localhost:9092",
    "group_id": argv[5] if len(argv) > 5 else "python_consumer_group1",
    "auto_offset_reset": "earliest",    # 'auto.offset.reset=earliest' to start reading from the beginning of the
                                        #   topic if no committed offsets exist
    "fetch_min_bytes": 1,
    "enable_auto_commit": "False",  # TODO: Update
    "value_deserializer" : lambda x: loads(str(x, encoding='utf-8')),
    "key_deserializer": lambda x: str(x, encoding='utf-8')
}

if __name__ == '__main__':
    consumer = KafkaConsumer(**CONSUMER_CONFIG)
    consumer.subscribe(topics=[TOPIC])
    try:
        while True:
            msg = consumer.poll(timeout_ms=TIMEOUT, max_records=MAX_RECORDS)
            if not msg:
                print("Waiting for message or event/error in poll()")
                continue
            else:
                for (k, records) in msg.items():
                    print(f"[{ k.topic }]")
                    for record in records:
                        print("{}: {}".format(record.key, record.value))
                consumer.commit_async()
    except KeyboardInterrupt:
        pass
    finally:
        consumer.close()