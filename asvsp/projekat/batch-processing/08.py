#!/usr/bin/python

# Countries globally ranked by their average production in the specified year span


from os import environ
from sys import argv
from pyspark.sql import SparkSession, Window
import pyspark.sql.functions as F
import pyspark.sql.types as T

spark = SparkSession \
    .builder \
    .appName("ASVSP - batch - 08") \
    .getOrCreate()

spark.sparkContext.setLogLevel("WARN")

HDFS_NAMENODE = environ.get("CORE_CONF_fs_defaultFS", "hdfs://namenode:9000")
CSV_FILEPATH = "/home/projekat/batch-dataset/output.csv"

YEAR_LOWER = int(argv[1] if len(argv) > 1 else "2015")
YEAR_UPPER = int(argv[2] if len(argv) > 2 else "2020")

print(f"Displaying globally ranked countries based on their average production in a specified year span '{YEAR_LOWER}' - '{YEAR_UPPER}'")

df = spark.read.csv(
  path=HDFS_NAMENODE + CSV_FILEPATH,
  header=True,
  inferSchema=True
  )

window_area = Window.partitionBy(F.col("Area")).orderBy(F.desc("Year"))
window_area_ranged = window_area.rangeBetween(Window.unboundedPreceding, Window.unboundedFollowing)

df.filter(
    (F.col("Unit") == "tonnes") &
    (F.col("Year").between(YEAR_LOWER, YEAR_UPPER))
).select(
    F.row_number().over(window_area).alias("Row"),
    F.col("Area"),
    F.avg("Value").over(window_area_ranged).alias("Average yield"),
    F.sum("Value").over(window_area_ranged).alias("Total yield"),
    F.col("Unit")
).filter(
    F.col("Row") == 1
).drop(F.col("Row"))\
    .orderBy([F.desc("Average yield"), F.asc("Area")])\
    .show(truncate=False)