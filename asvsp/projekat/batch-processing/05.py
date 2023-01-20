#!/usr/bin/python

# Products and countries producing them ranked by their average yield in the specified year span


from os import environ
from sys import argv
from pyspark.sql import SparkSession, Window
import pyspark.sql.functions as F
import pyspark.sql.types as T

spark = SparkSession \
    .builder \
    .appName("ASVSP - batch - 05") \
    .getOrCreate()

spark.sparkContext.setLogLevel("WARN")

HDFS_NAMENODE = environ.get("CORE_CONF_fs_defaultFS", "hdfs://namenode:9000")
CSV_FILEPATH = "/home/projekat/batch-dataset/output.csv"

RANK = int(argv[1] if len(argv) > 1 else "3")
YEAR_LOWER = int(argv[2] if len(argv) > 2 else "2010")
YEAR_UPPER = int(argv[3] if len(argv) > 3 else "2020")

print(f"Displaying results for the most produced items and countries producing them based on an average yield in the year span '{YEAR_LOWER}' - '{YEAR_UPPER}'")

df = spark.read.csv(
  path=HDFS_NAMENODE + CSV_FILEPATH,
  header=True,
  inferSchema=True
  )

window_area = Window.partitionBy([F.col("Area"), F.col("Item")]).orderBy("Year")
window_area_ranged = window_area.rangeBetween(Window.unboundedPreceding, Window.unboundedFollowing)

df.filter(
    (F.col("Unit") == "tonnes") &
    (F.col("Year").between(YEAR_LOWER, YEAR_UPPER))
).select(
    F.row_number().over(window_area).alias("Row"),
    F.col("Item"),
    F.col("Area"),
    F.avg(F.col("Value")).over(window_area_ranged).alias("Average yearly yield"),
    F.sum(F.col("Value")).over(window_area_ranged).alias("Total yield"),
    F.col("Unit"),
    F.collect_list("Year").over(window_area_ranged).alias("Year range")
).filter(
    F.col("Row") == 1
).orderBy([F.asc("Area"), F.asc("Item")])\
    .show(truncate=False)