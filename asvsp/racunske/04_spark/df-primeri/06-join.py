#!/usr/bin/python

from pyspark.sql import SparkSession, Row
from pyspark.sql.functions import col, lit, expr
from pyspark.sql.types import *

def quiet_logs(sc):
  logger = sc._jvm.org.apache.log4j
  logger.LogManager.getLogger("org"). setLevel(logger.Level.ERROR)
  logger.LogManager.getLogger("akka").setLevel(logger.Level.ERROR)

spark = SparkSession \
    .builder \
    .appName("Python Spark SQL - Join") \
    .getOrCreate()

quiet_logs(spark)

# TODO : optimizing joins

