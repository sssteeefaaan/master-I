package com.example.kafka_streams_examples;

import java.util.Properties;

import org.apache.kafka.common.serialization.Serdes;
import org.apache.kafka.streams.KafkaStreams;
import org.apache.kafka.streams.KeyValue;
import org.apache.kafka.streams.StreamsBuilder;
import org.apache.kafka.streams.kstream.*;

import com.example.kafka_streams_examples.util.KafkaConstants;
import com.example.kafka_streams_examples.util.KafkaStreamsUtil;

public class FlightNumberCounterExample {
	public static void main(String[] args) {
		final Properties streamsConfiguration = KafkaStreamsUtil.getStreamsConfiguration("flight-number-counter");

		// In the subsequent lines we define the processing topology of the Streams
		// application.
		// https://kafka.apache.org/23/javadoc/org/apache/kafka/streams/StreamsBuilder.html
		final StreamsBuilder builder = new StreamsBuilder();

		// Read the input Kafka topic into a KStream instance.
		final KStream<Long, String> flights = builder.stream(KafkaConstants.TOPIC_NAME);

		flights
				// https://kafka.apache.org/20/javadoc/org/apache/kafka/streams/kstream/KeyValueMapper.html
		.map(new KeyValueMapper<Long, String, KeyValue<String, Double>>() {
			@Override
			public KeyValue<String, Double> apply(Long key, String value) {
				String[] words = value.split(KafkaConstants.SEPARATOR);
				return new KeyValue<>(words[0], Double.parseDouble(words[9]));
			}
		})
		.groupByKey(Grouped.with(Serdes.String(), Serdes.Double()))
		.count()
		.toStream()
				.print(Printed.toSysOut());
		//.to(KafkaConstants.TOPIC_NAME + "-cnt", Produced.with(Serdes.String(), Serdes.Long())); // to send output on kafka topic

		@SuppressWarnings("resource")
		// Topology topology = builder.build();
		final KafkaStreams streams = new KafkaStreams(builder.build(), streamsConfiguration);

		// Do a clean up of the local StateStore directory (StreamsConfig.STATE_DIR_CONFIG) by deleting all data with regard to the application ID.
		streams.cleanUp();

		// Start the KafkaStreams instance by starting all its threads.
		streams.start();

		// Add shutdown hook to respond to SIGTERM and gracefully close Kafka Streams
		Runtime.getRuntime().addShutdownHook(new Thread(streams::close));
	}
}
