#!/usr/bin/python

# Countries production change in a specified year span

# /spark/bin/spark-submit --packages org.elasticsearch:elasticsearch-spark-30_2.12:8.6.0 10.py

from os import environ
from sys import argv
from pyspark.sql import SparkSession, Window
import pyspark.sql.functions as F
import pyspark.sql.types as T

spark = SparkSession \
    .builder \
    .appName("ASVSP - batch - 10") \
    .getOrCreate()

spark.sparkContext.setLogLevel("WARN")

HDFS_NAMENODE = environ.get("CORE_CONF_fs_defaultFS", "hdfs://namenode:9000")
CSV_FILEPATH = "/home/projekat/batch-dataset/output.csv"

ELASTIC_SEARCH_NODE = environ.get("ELASTIC_SEARCH_NODE", "elasticsearch")
ELASTIC_SEARCH_USERNAME = environ.get("ELASTIC_SEARCH_USERNAME", "elastic")
ELASTIC_SEARCH_PASSWORD = environ.get("ELASTIC_SEARCH_PASSWORD", "changeme")
ELASTIC_SEARCH_PORT = environ.get("ELASTIC_SEARCH_PORT", "9200")

YEAR_LOWER = int(argv[1] if len(argv) > 1 else "2015")
YEAR_UPPER = int(argv[2] if len(argv) > 2 else "2020")
ELASTIC_SEARCH_INDEX = argv[3] if len(argv) > 3 else "batch-10-query"

print(f"Displaying ranked countries based on their production change in a specified year span '{YEAR_LOWER}' - '{YEAR_UPPER}'")

df = spark.read.csv(
  path=HDFS_NAMENODE + CSV_FILEPATH,
  header=True,
  inferSchema=True
  )

window = Window.partitionBy(F.col("Area")).orderBy(F.asc("Year"))
window_year = Window.partitionBy([F.col("Area"), F.col("Year")])

display = df.filter(
    (F.col("Unit") == "tonnes") &
    (F.col("Year").between(YEAR_LOWER, YEAR_UPPER))
).select(
    F.row_number().over(window_year.orderBy("Item")).alias("Row"),
    F.col("Area"),
    F.col("Area Code (M49)"),
    F.col("Year"),
    (F.col("Year") - 1).alias("Previous year"),
    (F.sum("Value").over(window_year)).alias("Yearly yield"),
    F.col("Unit")
).filter(
    F.col("Row") == 1
).select(
    F.col("Area"),
    F.col("Area Code (M49)"),
    F.col("Year"),
    F.col("Previous year"),
    F.col("Yearly yield"),
    (F.col("Yearly yield") - F.lag(F.col("Yearly yield"), 1, 0).over(window)).alias("Change"),
    F.col("Unit")
)\
.orderBy([F.desc("Year"), F.desc("Change"), F.asc("Area")])

display.show(truncate=False)

display.select(
    F.row_number().over(Window.partitionBy("Year").orderBy(F.desc("Change"))).alias("Rank"),
    F.col("Area"),
    F.col("Area Code (M49)"),
    F.col("Year"),
    F.col("Previous year"),
    F.col("Yearly yield"),
    (F.col("Yearly yield") - F.lag(F.col("Yearly yield"), 1, 0).over(window)).alias("Change"),
    F.col("Unit")
).filter(
    F.col("Rank") < 4
).write\
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