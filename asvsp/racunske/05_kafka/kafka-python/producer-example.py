from confluent_kafka import Producer, KafkaError
import json

TOPIC = 'python_topic'

CONSUMER_CONFIG = {
    "bootstrap.servers": "localhost:9092",
    "group.id": "python_consumer_group1",
    "auto.offset.reset": "earliest",    # 'auto.offset.reset=earliest' to start reading from the beginning of the
                                        #   topic if no committed offsets exist
    "fetch.min.bytes": "1",
    "enable.auto.commit": "True",  # TODO: Update
}

PRODUCER_CONFIG = {
    "bootstrap.servers": "localhost:9092",
    "message.max.bytes": 100000,
}


if __name__ == '__main__':

    producer = Producer(PRODUCER_CONFIG)
    delivered_records = 0

    def hash(key):
        return len(key)-1

    # Optional per-message on_delivery handler (triggered by poll() or flush())
    # when a message has been successfully delivered or
    # permanently failed delivery (after retries).
    def acked(err, msg):
        global delivered_records
        """Delivery report handler called on
        successful or failed delivery of message
        """
        if err is not None:
            print("Failed to deliver message: {}".format(err))
        else:
            delivered_records += 1
            print("Produced record with key {} to topic {} partition [{}] @ offset {}"
                  .format(msg.key(), msg.topic(), msg.partition(), msg.offset()))


    with open('prepared_messages.txt') as f:
        lines = f.readlines()

        for l in lines:
            line_split = l.split(':')
            record_key = line_split[0]
            record_value = line_split[1]

            print("Producing record: {}\t{}".format(record_key, record_value))
            producer.produce(TOPIC, key=record_key, value=record_value, partition=hash(record_key),on_delivery=acked)
            # p.poll() serves delivery reports (on_delivery)
            # from previous produce() calls.
            producer.poll(0)

        producer.flush()

        print("{} messages were produced to topic {}!".format(delivered_records, TOPIC))

