#!/usr/bin/python3

import os
from pyspark import SparkContext, SparkConf

HDFS_NAMENODE = os.environ["CORE_CONF_fs_defaultFS"]

conf = SparkConf().setAppName("toplongwords").setMaster("local")
sc = SparkContext(conf=conf)

text_file = sc.textFile(HDFS_NAMENODE + "/dante/divine_comedy.txt")
counts = text_file \
    .flatMap(lambda line: line.split(" ")) \
    .filter(lambda word: len(word) > 5) \
    .map(lambda word: (word, 1)) \
    .reduceByKey(lambda a, b: a + b) \
    .filter(lambda pair: pair[1] > 50)
counts.saveAsTextFile(HDFS_NAMENODE + "/dante/top")