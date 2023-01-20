#!/usr/bin/python

# Products globally ranked by their average yield in the specified year span


from os import environ
from sys import argv
from pyspark.sql import SparkSession, Window
import pyspark.sql.functions as F
import pyspark.sql.types as T

spark = SparkSession \
    .builder \
    .appName("ASVSP - batch - 07") \
    .getOrCreate()

spark.sparkContext.setLogLevel("WARN")

HDFS_NAMENODE = environ.get("CORE_CONF_fs_defaultFS", "hdfs://namenode:9000")
CSV_FILEPATH = "/home/projekat/batch-dataset/output.csv"

YEAR_LOWER = int(argv[1] if len(argv) > 1 else "2015")
YEAR_UPPER = int(argv[2] if len(argv) > 2 else "2020")

print(f"Displaying globally ranked products based on their average yield in a specified year span '{YEAR_LOWER}' - '{YEAR_UPPER}'")

df = spark.read.csv(
  path=HDFS_NAMENODE + CSV_FILEPATH,
  header=True,
  inferSchema=True
  )

window_item = Window.partitionBy(F.col("Item")).orderBy(F.desc("Year"))
window_item_ranged = window_item.rangeBetween(Window.unboundedPreceding, Window.unboundedFollowing)

df.filter(
    (F.col("Unit") == "tonnes") &
    (F.col("Year").between(YEAR_LOWER, YEAR_UPPER))
).select(
    F.row_number().over(window_item).alias("Row"),
    F.col("Item"),
    F.avg("Value").over(window_item_ranged).alias("Average yield"),
    F.sum("Value").over(window_item_ranged).alias("Total yield"),
    F.col("Unit")
).filter(
    F.col("Row") == 1
).drop(F.col("Row"))\
    .orderBy([F.desc("Average yield"), F.asc("Item")])\
    .show(truncate=False)