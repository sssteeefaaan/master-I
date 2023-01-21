#!/usr/bin/python

# Ranking countries based on their percentage of global yield for a specified item in a specified year range

from os import environ
from sys import argv
from pyspark.sql import SparkSession, Window
import pyspark.sql.functions as F
import pyspark.sql.types as T

spark = SparkSession \
    .builder \
    .appName("ASVSP - batch - 02") \
    .getOrCreate()

spark.sparkContext.setLogLevel("WARN")

HDFS_NAMENODE = environ.get("CORE_CONF_fs_defaultFS", "hdfs://namenode:9000")
CSV_FILEPATH = "/home/project/raw-layer/batch-dataset/output.csv"
OUTPUT_PATH = "/home/project/transform-layer/batch/02.csv"

ITEM = argv[1] if len(argv) > 1 else "Apples"
YEAR_LOWER = int(argv[2] if len(argv) > 2 else "2017")
YEAR_UPPER = int(argv[3] if len(argv) > 3 else "2020")

print(f"Displaying results for ranked countries based on their global yield value percentage of '{ITEM}' in a year span '{YEAR_LOWER}' - '{YEAR_UPPER}'")

df = spark.read.csv(
  path=HDFS_NAMENODE + CSV_FILEPATH,
  header=True,
  inferSchema=True
  )


window_area = Window.partitionBy([F.col("Area"), F.col("Item"), F.col("Unit")]).orderBy("Year")
window_area_full = window_area.rangeBetween(Window.unboundedPreceding, Window.unboundedFollowing)
window_global = Window.partitionBy([F.col("Item"), F.col("Unit")])\
    .orderBy("Value")\
    .rangeBetween(Window.unboundedPreceding, Window.unboundedFollowing)

display = df.filter(
    (F.col("Year").between(YEAR_LOWER, YEAR_UPPER)) &
    (F.col("Item") == ITEM) &
    (F.col("Unit") == "tonnes")
).select(
    F.row_number().over(window_area).alias("Row"),
    F.col("Area"),
    F.col("Item"),
    F.collect_list("Year").over(window_area_full).alias("Year range"),
    F.sum("Value").over(window_area_full).alias("Area Total"),
    F.sum("Value").over(window_global).alias("Global Total"),
    (F.sum("Value").over(window_area_full) / F.sum("Value").over(window_global)).alias("Percentage"),
    F.col("Unit")
).filter(
    F.col("Row") == 1
).drop("Row")\
    .orderBy(F.desc("Percentage"))

display.show(truncate=False)

print(f"Saving to '{HDFS_NAMENODE}{OUTPUT_PATH}'")

display.withColumn("Year range", F.col("Year range").cast(T.StringType()))\
    .write.csv(HDFS_NAMENODE+OUTPUT_PATH)

print("Saving completed!")