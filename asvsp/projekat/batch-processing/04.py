#!/usr/bin/python

# Countries order by their minimum and maximum yearly yield for a specified year span


from os import environ
from sys import argv
from pyspark.sql import SparkSession, Window
import pyspark.sql.functions as F
import pyspark.sql.types as T

spark = SparkSession \
    .builder \
    .appName("ASVSP - batch - 04") \
    .getOrCreate()

spark.sparkContext.setLogLevel("WARN")

HDFS_NAMENODE = environ.get("CORE_CONF_fs_defaultFS", "hdfs://namenode:9000")
CSV_FILEPATH = "/home/projekat/batch-dataset/output.csv"

YEAR_LOWER = int(argv[1] if len(argv) > 1 else "2000")
YEAR_UPPER = int(argv[2] if len(argv) > 2 else "2020")

print(f"Displaying results for the countries with most and least yearly yield for a year span '{YEAR_LOWER}-{YEAR_UPPER}'")

df = spark.read.csv(
  path=HDFS_NAMENODE + CSV_FILEPATH,
  header=True,
  inferSchema=True
  )

window_area = Window.partitionBy([F.col("Area"), F.col("Year")])
window_year = Window.partitionBy([F.col("Year")])

df.filter(
    (F.col("Unit") == "tonnes") &
    (F.col("Year").between(YEAR_LOWER, YEAR_UPPER))
).select(
    F.col("Year"),
    F.col("Area"),
    F.sum("Value").over(window_area).alias("Total by Area"),
    F.sum("Value").over(window_year).alias("Total globally"),
    (F.sum("Value").over(window_area) / F.sum("Value").over(window_year)).alias("Percentage"),
    F.col("Unit")
).select(
    F.col("Year"),
    F.row_number().over(window_year.orderBy(F.desc("Total by Area"))).alias("Max rank"),
    F.row_number().over(window_year.orderBy(F.asc("Total by Area"))).alias("Min rank"),
    F.col("Area"),
    F.col("Total by Area"),
    F.col("Total globally"),
    F.col("Percentage"),
    F.col("Unit")
).filter(
    (F.col("Max rank") == 1) |
    (F.col("Min rank") == 1)
).orderBy([F.desc("Year")])\
    .show(truncate=False)