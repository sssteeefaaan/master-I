#!/usr/bin/python

# Countries ranked based on their yield of a specified product in a specified year

# /spark/bin/spark-submit --packages org.elasticsearch:elasticsearch-spark-30_2.12:8.6.0 01.py

from os import environ
from sys import argv
from pyspark.sql import SparkSession, Window
import pyspark.sql.functions as F
import pyspark.sql.types as T

spark = SparkSession \
    .builder \
    .appName("ASVSP - batch - 01") \
    .getOrCreate()


spark.sparkContext.setLogLevel("WARN")

HDFS_NAMENODE = environ.get("CORE_CONF_fs_defaultFS", "hdfs://namenode:9000")
CSV_FILEPATH = "/home/projekat/batch-dataset/output.csv"

ELASTIC_SEARCH_NODE = environ.get("ELASTIC_SEARCH_NODE", "elasticsearch")
ELASTIC_SEARCH_USERNAME = environ.get("ELASTIC_SEARCH_USERNAME", "elastic")
ELASTIC_SEARCH_PASSWORD = environ.get("ELASTIC_SEARCH_PASSWORD", "changeme")
ELASTIC_SEARCH_PORT = environ.get("ELASTIC_SEARCH_PORT", "9200")

ITEM = argv[1] if len(argv) > 1 else "Apples"
YEAR = int(argv[2] if len(argv) > 2 else "2020")
ELASTIC_SEARCH_INDEX = argv[3] if len(argv) > 3 else "batch-01-query"

print(f"Displaying results for ranked countries based on their yield value of '{ITEM}' in '{YEAR}'")

df = spark.read.csv(
  path=HDFS_NAMENODE + CSV_FILEPATH,
  header=True,
  inferSchema=True
  )

window = Window.partitionBy([F.col("Item"), F.col("Year"), F.col("Unit")]).orderBy(F.desc("Value"))

display = df.select(
    F.dense_rank().over(window).alias("Rank"),
    F.col("Area Code (M49)"),
    F.col("Area"),
    F.col("Year"),
    F.col("Item"),
    F.col("Value"),
    F.col("Unit")
).filter(
    (F.col("Item") == ITEM) &
    (F.col("Year") == YEAR) &
    (F.col("Unit") == "tonnes")
).orderBy(
    F.asc("Rank")
)

display.show(truncate=False)

display.limit(100)\
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

