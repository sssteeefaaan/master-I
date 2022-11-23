#!/usr/bin/python3

from __future__ import print_function
from pyspark import SparkContext, SparkConf

def quiet_logs(sc):
  logger = sc._jvm.org.apache.log4j
#   logger.LogManager.getLogger("org").setLevel(logger.Level.ERROR)
#   logger.LogManager.getLogger("akka").setLevel(logger.Level.ERROR)
  logger.LogManager.getRootLogger().setLevel(logger.Level.ERROR)

conf = SparkConf().setAppName("groupsortjoin").setMaster("local")
sc = SparkContext(conf=conf)
quiet_logs(sc)

data = sc.parallelize([('k',5),('s',3),('s',4),('p',7),('p',5),('t',8),('k',6)],3)
group = data.groupByKey().mapValues(lambda x : list(x))
print("GROUP BY KEY:")
group.foreach(print)

sorted = data.sortByKey()
print("SORT BY KEY:")
sorted.foreach(print)

data = sc.parallelize([('A',1),('b',2),('c',3)])
data2 = sc.parallelize([('A',4),('A',6),('b',7),('c',3),('c',8)])
result = data.join(data2)
print("JOIN:")
print(result.collect())

rdd1 = sc.parallelize(["jan","feb","mar","apr","may","jun"],3)
result = rdd1.coalesce(2) # coalesce will decrease the number of partitions of the source RDD to numPartitions define in coalesce argument
print("COALESCE:")
result.foreach(print)