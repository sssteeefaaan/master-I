#!/usr/bin/python3

from __future__ import print_function
from pyspark import SparkContext, SparkConf

def quiet_logs(sc):
  logger = sc._jvm.org.apache.log4j
#   logger.LogManager.getLogger("org").setLevel(logger.Level.ERROR)
#   logger.LogManager.getLogger("akka").setLevel(logger.Level.ERROR)
  logger.LogManager.getRootLogger().setLevel(logger.Level.ERROR)

conf = SparkConf().setAppName("setops").setMaster("local")
sc = SparkContext(conf=conf)
quiet_logs(sc)

rdd1 = sc.parallelize([(1,"jan",2016),(3,"nov",2014),(16,"feb",2014)])
rdd2 = sc.parallelize([(5,"dec",2014),(17,"sep",2015)])
rdd3 = sc.parallelize([(6,"dec",2011),(16,"may",2015)])
rddUnion = rdd1.union(rdd2).union(rdd3)
print("UNION:")
rddUnion.foreach(print)

rdd1 = sc.parallelize([(1,"jan",2016),(3,"nov",2014), (16,"feb",2014)])
rdd2 = sc.parallelize([(5,"dec",2014),(1,"jan",2016)])
rddIntersection = rdd1.intersection(rdd2)
print("INTERSECTION:")
rddIntersection.foreach(print)

rdd1 = sc.parallelize([(1,"jan",2016),(3,"nov",2014),(16,"feb",2014),(3,"nov",2014)])
result = rdd1.distinct()
print("DISTINCT:")
print(result.collect())