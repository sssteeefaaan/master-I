#!/usr/bin/python

# Products and countries producing them ranked by their minimal and maximal average yield in the specified year span


from os import environ
from sys import argv
from pyspark.sql import SparkSession, Window
import pyspark.sql.functions as F
import pyspark.sql.types as T


spark = SparkSession \
    .builder \
    .appName("ASVSP - batch - 06") \
    .getOrCreate()

spark.sparkContext.setLogLevel("WARN")

HDFS_NAMENODE = environ.get("CORE_CONF_fs_defaultFS", "hdfs://namenode:9000")
CSV_FILEPATH = "/home/projekat/batch-dataset/output.csv"

YEAR_LOWER = int(argv[1] if len(argv) > 1 else "2015")
YEAR_UPPER = int(argv[2] if len(argv) > 2 else "2020")

print(f"Displaying results for the maximum and minimum average yield for every country in range '{YEAR_LOWER}' - '{YEAR_UPPER}'")

df = spark.read.csv(
  path=HDFS_NAMENODE + CSV_FILEPATH,
  header=True,
  inferSchema=True
  )

window_item = Window.partitionBy([F.col("Item"), F.col("Area")])
window_item_ranged = window_item.orderBy(F.desc("Year")).rangeBetween(Window.unboundedPreceding, Window.unboundedFollowing)
window_area = Window.partitionBy(F.col("Area"))


df.filter(
    (F.col("Unit") == "tonnes") &
    (F.col("Year").between(YEAR_LOWER, YEAR_UPPER))
).select(
    F.col("Item"),
    F.col("Area"),
    F.avg("Value").over(window_item_ranged).alias("Average yield"),
    F.sum("Value").over(window_item_ranged).alias("Total yield"),
    F.col("Unit")
).select(
    F.row_number().over(window_area.orderBy(F.desc("Average yield"))).alias("Max average yield"),
    F.row_number().over(window_area.orderBy(F.asc("Average yield"))).alias("Min average yield"),
    F.col("Item"),
    F.col("Area"),
    F.col("Average yield"),
    F.col("Total yield"),
    F.col("Unit")
).filter(
    (F.col("Max average yield") == 1) |
    (F.col("Min average yield") == 1)
).orderBy([F.asc("Area"), F.asc("Item"), F.asc("Average yield")])\
    .show(truncate=False)