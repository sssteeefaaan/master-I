import logging

from confluent_kafka import TIMESTAMP_NOT_AVAILABLE
from confluent_kafka import Consumer
from confluent_kafka import KafkaError
from confluent_kafka import Message
from confluent_kafka import Producer
from confluent_kafka.cimpl import KafkaException

TOPIC = 'python_topic'
TIMEOUT=1 # Maximum time to block waiting for message, event or callback

CONSUMER_CONFIG = {
    "bootstrap.servers": "localhost:9092",
    "group.id": "python_consumer_group1",
    "auto.offset.reset": "earliest",    # 'auto.offset.reset=earliest' to start reading from the beginning of the
                                        #   topic if no committed offsets exist
    "fetch.min.bytes": "1",
    "enable.auto.commit": "False",  # TODO: Update
}

if __name__ == '__main__':
    consumer = Consumer(CONSUMER_CONFIG)
    consumer.subscribe([TOPIC])

    # Process messages
    try:
        while True:
            msg = consumer.poll(TIMEOUT)
            if msg is None:
                # No message available within timeout.
                # Initial message consumption may take up to
                # `session.timeout.ms` for the consumer group to
                # rebalance and start consuming
                print("Waiting for message or event/error in poll()")
                continue
            elif msg.error():
                print('error: {}'.format(msg.error()))
            else:
                # Check for Kafka message
                record_key = msg.key()
                record_value = msg.value()
                partition = msg.partition()
                offset = msg.offset()
                print("Key: {}, Value: {}, Partition: {}, Offset: {}"
                      .format(record_key, record_value, partition, offset))
                consumer.commit(asynchronous=False)  # Synchronous guarantees, Asynchronous will speed up
    except KeyboardInterrupt:
        pass
    finally:
        # Leave group and commit final offsets
        consumer.close()