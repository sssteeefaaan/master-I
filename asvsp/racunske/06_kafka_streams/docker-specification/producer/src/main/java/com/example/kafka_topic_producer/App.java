package com.example.kafka_topic_producer;

import java.io.*;
import java.util.StringJoiner;
import java.util.concurrent.ExecutionException;

import org.apache.kafka.clients.producer.Producer;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.kafka.clients.producer.RecordMetadata;

import com.opencsv.CSVReader;
import com.opencsv.CSVReaderBuilder;

public class App {
    public static void main(String[] args) {
        try (InputStream inputStream = new App().getClass().getResourceAsStream("/pek-sha.csv");) {
            readOneByOneAndProduce(new InputStreamReader(inputStream), ProducerCreator.createProducer());
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    static void readOneByOneAndProduce(Reader reader, Producer<Long, String> producer) {
        try (CSVReader csvReader = new CSVReaderBuilder(reader).withSkipLines(1).build()) {
            String[] line;
            while ((line = csvReader.readNext()) != null) {
                StringJoiner joiner = new StringJoiner("#");
                for (int i = 1; i < line.length; i++) {
                    joiner.add(line[i]);
                }
                ProducerRecord<Long, String> record = new ProducerRecord<Long, String>(KafkaConstants.TOPIC_NAME,
                        Long.parseLong(line[0]), joiner.toString());
                try {
                    RecordMetadata metadata = producer.send(record).get();
                    System.out.println("Record sent with key " + line[0] + " to partition " + metadata.partition()
                            + " with offset " + metadata.offset());
                    Thread.sleep(300);
                } catch (ExecutionException e) {
                    System.out.println("Error in sending record");
                    System.out.println(e);
                } catch (InterruptedException e) {
                    System.out.println("Error in sending record");
                    System.out.println(e);
                }
            }
            reader.close();
        } catch (IOException e) {
            System.out.println("I/O error occured");
        }
    }
}
