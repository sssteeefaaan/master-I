#!/usr/bin/python3

# Maximum bid for a commodity in a time range

#/spark/bin/spark-submit --packages org.apache.spark:spark-sql-kafka-0-10_2.12:3.0.1 01.py

from pyspark.sql import SparkSession
import pyspark.sql.functions as F
import pyspark.sql.types as T

spark = SparkSession \
    .builder \
    .appName("ASVSP - realtime - 01") \
    .getOrCreate()

spark.sparkContext.setLogLevel("ERROR")

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
    .groupBy(F.window("timestamp", "5 seconds", "2 seconds"), "commodity")\
    .max("bid")

query = max_bid_stream.writeStream \
    .outputMode("complete") \
    .format("console") \
    .option("truncate", "false") \
    .start()

query.awaitTermination()