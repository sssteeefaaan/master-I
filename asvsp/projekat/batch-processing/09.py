#!/usr/bin/python

# Items production change in a specified year span

from os import environ
from sys import argv
from pyspark.sql import SparkSession, Window
import pyspark.sql.functions as F
import pyspark.sql.types as T

spark = SparkSession \
    .builder \
    .appName("ASVSP - batch - 09") \
    .getOrCreate()

spark.sparkContext.setLogLevel("WARN")

HDFS_NAMENODE = environ.get("CORE_CONF_fs_defaultFS", "hdfs://namenode:9000")
CSV_FILEPATH = "/home/projekat/batch-dataset/output.csv"

YEAR_LOWER = int(argv[1] if len(argv) > 1 else "2015")
YEAR_UPPER = int(argv[2] if len(argv) > 2 else "2020")

print(f"Displaying ranked items based on their production change in a specified year span '{YEAR_LOWER}' - '{YEAR_UPPER}'")

df = spark.read.csv(
  path=HDFS_NAMENODE + CSV_FILEPATH,
  header=True,
  inferSchema=True
  )

window = Window.partitionBy(F.col("Item")).orderBy(F.asc("Year"))
window_year = Window.partitionBy([F.col("Item"), F.col("Year")])

df.filter(
    (F.col("Unit") == "tonnes") &
    (F.col("Year").between(YEAR_LOWER, YEAR_UPPER))
).select(
    F.row_number().over(window_year.orderBy("Area")).alias("Row"),
    F.col("Item"),
    F.col("Year"),
    (F.col("Year") - 1).alias("Previous year"),
    (F.sum("Value").over(window_year)).alias("Yearly yield"),
    F.col("Unit")
).filter(
    F.col("Row") == 1
).select(
    F.col("Item"),
    F.col("Year"),
    F.col("Previous year"),
    F.col("Yearly yield"),
    (F.col("Yearly yield") - F.lag(F.col("Yearly yield"), 1, 0).over(window)).alias("Change"),
    F.col("Unit")
)\
.orderBy([F.desc("Year"), F.desc("Change"), F.asc("Item")])\
    .show(truncate=False)