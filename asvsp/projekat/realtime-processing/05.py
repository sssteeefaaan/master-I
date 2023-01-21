#!/usr/bin/python3

# Joined processing, combining city names and geo-spatial points for bids

# /spark/bin/spark-submit --packages org.apache.spark:spark-sql-kafka-0-10_2.12:3.0.1,\org.elasticsearch:elasticsearch-spark-30_2.12:8.6.0 05.py

from pyspark.sql import SparkSession
import pyspark.sql.functions as F
import pyspark.sql.types as T
from sys import argv
from os import environ

HDFS_NAMENODE = environ.get("CORE_CONF_fs_defaultFS", "hdfs://namenode:9000")
CSV_FILEPATH = "/home/project/raw-layer/batch-dataset/uscities.csv"

spark = SparkSession \
    .builder \
    .appName("ASVSP - realtime - 05") \
    .getOrCreate()

spark.sparkContext.setLogLevel("ERROR")

ELASTIC_SEARCH_NODE = environ.get("ELASTIC_SEARCH_NODE", "elasticsearch")
ELASTIC_SEARCH_USERNAME = environ.get("ELASTIC_SEARCH_USERNAME", "elastic")
ELASTIC_SEARCH_PASSWORD = environ.get("ELASTIC_SEARCH_PASSWORD", "changeme")
ELASTIC_SEARCH_PORT = environ.get("ELASTIC_SEARCH_PORT", "9200")

ELASTIC_SEARCH_INDEX = argv[1] if len(argv) > 1 else "realtime-05-query"

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
    .withColumn("bid", F.col("bid").cast(T.DoubleType()))\
    .withColumnRenamed("source", "event_source")\
    .withWatermark("timestamp", "1 seconds")\

cities = spark.read.csv(
  path=HDFS_NAMENODE + CSV_FILEPATH,
  header=True,
  inferSchema=True
  )

city_ascii = argv[1] if len(argv) > 1 else "New York"

joined = prices.join(cities, (prices.longitude.between(cities.lng - 1, cities.lng + 1)) & (prices.latitude.between(cities.lat -1, cities.lat + 1)))

def process(row: T.Row):
    entry = row.asDict()
    if entry.get("city_ascii", None):
        print(f"{row['city_ascii']} is offering '{row['commodity']}' for '{row['bid']}'!")

joined.writeStream\
    .foreach(process)\
    .outputMode("append")\
    .start()

es_query = joined.writeStream\
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