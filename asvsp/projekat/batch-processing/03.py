#!/usr/bin/python

# Top x most produced items and countries producing them for every year in the specified year span

# /spark/bin/spark-submit --packages org.elasticsearch:elasticsearch-spark-30_2.12:8.6.0 03.py

from os import environ
from sys import argv
from pyspark.sql import SparkSession, Window
import pyspark.sql.functions as F
import pyspark.sql.types as T

spark = SparkSession \
    .builder \
    .appName("ASVSP - batch - 03") \
    .getOrCreate()

spark.sparkContext.setLogLevel("WARN")

HDFS_NAMENODE = environ.get("CORE_CONF_fs_defaultFS", "hdfs://namenode:9000")
CSV_FILEPATH = "/home/project/raw-layer/batch-dataset/output.csv"
OUTPUT_PATH = "/home/project/transform-layer/batch/03.csv"

ELASTIC_SEARCH_NODE = environ.get("ELASTIC_SEARCH_NODE", "elasticsearch")
ELASTIC_SEARCH_USERNAME = environ.get("ELASTIC_SEARCH_USERNAME", "elastic")
ELASTIC_SEARCH_PASSWORD = environ.get("ELASTIC_SEARCH_PASSWORD", "changeme")
ELASTIC_SEARCH_PORT = environ.get("ELASTIC_SEARCH_PORT", "9200")

RANK = int(argv[1] if len(argv) > 1 else "3")
YEAR_LOWER = int(argv[2] if len(argv) > 2 else "2015")
YEAR_UPPER = int(argv[3] if len(argv) > 3 else "2020")
ELASTIC_SEARCH_INDEX = argv[4] if len(argv) > 4 else "batch-03-query"

print(f"Displaying results for the top {RANK} most produced items and countries producing them yearly in range '{YEAR_LOWER}' - '{YEAR_UPPER}'")

df = spark.read.csv(
  path=HDFS_NAMENODE + CSV_FILEPATH,
  header=True,
  inferSchema=True
  )

window_item = Window.partitionBy([F.col("Item"), F.col("Year")])\
    .orderBy(F.desc("Value"))
window_year = Window.partitionBy([F.col("Year")])\
    .orderBy(F.desc("Value"))

display = df.filter(
    (F.col("Unit") == "tonnes") &
    (F.col("Year").between(YEAR_LOWER, YEAR_UPPER))
).select(
    F.row_number().over(window_item).alias("Rank_item"),
    F.col("Year"),
    F.col("Item"),
    F.col("Value"),
    F.col("Unit"),
    F.col("Area")
).filter(
    F.col("Rank_item") == 1
).select(
    F.col("Year"),
    F.row_number().over(window_year).alias("Rank"),
    F.col("Item"),
    F.col("Value"),
    F.col("Unit"),
    F.col("Area")
).filter(
    F.col("Rank") <= RANK
).orderBy([F.desc("Year"), F.asc("Rank")]).limit(RANK * (YEAR_UPPER-YEAR_LOWER))

display.show(truncate=False)

print(f"Saving to '{HDFS_NAMENODE}{OUTPUT_PATH}'")

display.write.csv(HDFS_NAMENODE+OUTPUT_PATH)

print("Saving completed!")

print(f"Writing data to 'http://{ELASTIC_SEARCH_NODE}:{ELASTIC_SEARCH_PORT}/{ELASTIC_SEARCH_INDEX}'")

display\
    .write\
    .mode("append")\
    .format('org.elasticsearch.spark.sql') \
    .option("es.net.http.auth.user", ELASTIC_SEARCH_USERNAME) \
    .option("es.net.http.auth.pass", ELASTIC_SEARCH_PASSWORD) \
    .option("mergeSchema", "true") \
    .option('es.index.auto.create', 'true') \
    .option('es.nodes', f'http://{ELASTIC_SEARCH_NODE}') \
    .option('es.port', ELASTIC_SEARCH_PORT) \
    .option('es.batch.write.retry.wait', '100s') \
    .save(ELASTIC_SEARCH_INDEX)

print("Done!")