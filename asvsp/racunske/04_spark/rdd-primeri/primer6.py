#!/usr/bin/python3

from __future__ import print_function
from pyspark import SparkContext, SparkConf

def quiet_logs(sc):
  logger = sc._jvm.org.apache.log4j
#   logger.LogManager.getLogger("org").setLevel(logger.Level.ERROR)
#   logger.LogManager.getLogger("akka").setLevel(logger.Level.ERROR)
  logger.LogManager.getRootLogger().setLevel(logger.Level.ERROR)

conf = SparkConf().setAppName("sharedvars").setMaster("local")
sc = SparkContext(conf=conf)
quiet_logs(sc)

broadcastVar1 = sc.broadcast([1, 2, 3])
print("BROADCASTED VARIABLE:")
print(broadcastVar1.value)

accum1 = sc.accumulator(0)
sc.parallelize([1, 2, 3, 4]).foreach(lambda x: accum1.add(x))
print("ACCUMULATOR VALUE:")
print(accum1.value)