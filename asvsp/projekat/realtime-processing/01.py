#!/usr/bin/python3

# Maximum bid for a commodity in a time range

# /spark/bin/spark-submit --packages org.apache.spark:spark-sql-kafka-0-10_2.12:3.0.1,\org.elasticsearch:elasticsearch-spark-30_2.12:8.6.0 01.py

from pyspark.sql import SparkSession
import pyspark.sql.functions as F
import pyspark.sql.types as T
from os import environ
from sys import argv

spark = SparkSession \
    .builder \
    .appName("ASVSP - realtime - 01") \
    .getOrCreate()

spark.sparkContext.setLogLevel("ERROR")

ELASTIC_SEARCH_NODE = environ.get("ELASTIC_SEARCH_NODE", "elasticsearch")
ELASTIC_SEARCH_USERNAME = environ.get("ELASTIC_SEARCH_USERNAME", "elastic")
ELASTIC_SEARCH_PASSWORD = environ.get("ELASTIC_SEARCH_PASSWORD", "changeme")
ELASTIC_SEARCH_PORT = environ.get("ELASTIC_SEARCH_PORT", "9200")

ELASTIC_SEARCH_INDEX = argv[1] if len(argv) > 1 else "realtime-01-query"

schema = T.StructType([
                T.StructField("date", T.DateType(), True),
                T.StructField("week", T.StringType(), True),
                T.StructField("month", T.StringType(), True),
                T.StructField("year", T.StringType(), True),
                T.StructField("market_name", T.StringType(), True),
                T.StructField("market_type", T.StringType(), True),
                T.StructField("commodity", T.StringType(), True),
                T.StructField("source", T.StringType(), True),
                T.StructField("latitude", T.StringType(), True),
                T.StructField("longitude", T.StringType(), True),
                T.StructField("geocoded_column", T.StructType(
                    [
                        T.StructField("type", T.StringType(), True),
                        T.StructField("coordinates", T.ArrayType(T.DoubleType()), True)
                    ]
                    ), True),
                T.StructField("bid", T.StringType(), True)
            ])

prices = spark \
  .readStream \
  .format("kafka") \
  .option("kafka.bootstrap.servers", "kafka:9092") \
  .option("subscribe", "grain-prices") \
  .load()\
  .withColumn("parsed_value", F.from_json(F.col("value").cast("string"), schema))\
  .select(
    F.col("timestamp"),
    F.col("key").cast("string"),
    F.col("parsed_value.*")
  )\
    .withColumn("longitude", F.col("longitude").cast(T.DoubleType()))\
    .withColumn("latitude", F.col("latitude").cast(T.DoubleType()))\
    .withColumn("bid", F.col("bid").cast(T.DoubleType()))

max_bid_stream = prices\
    .withWatermark("timestamp", "1 seconds")\
    .groupBy(F.window("timestamp", "5 seconds", "2 seconds"), "commodity")\
    .max("bid")

query = max_bid_stream.writeStream \
    .outputMode("complete") \
    .format("console") \
    .option("truncate", "false") \
    .start()

es_query = max_bid_stream.writeStream\
    .outputMode("append")\
    .option("checkpointLocation", "/tmp/") \
    .format('org.elasticsearch.spark.sql') \
    .option("es.net.http.auth.user", ELASTIC_SEARCH_USERNAME) \
    .option("es.net.http.auth.pass", ELASTIC_SEARCH_PASSWORD) \
    .option("mergeSchema", "true") \
    .option('es.index.auto.create', 'true') \
    .option('es.nodes', f'http://{ELASTIC_SEARCH_NODE}') \
    .option('es.port', ELASTIC_SEARCH_PORT) \
    .option('es.batch.write.retry.wait', '100s') \
    .start(ELASTIC_SEARCH_INDEX)

spark.streams.awaitAnyTermination()