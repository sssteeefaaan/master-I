#!/bin/sh

DEFAULT_HDFS_DATASET_PATH="/home/projekat/batch-dataset"
DEFAULT_NAMENODE_DATASET_PATH="/home/projekat/batch-dataset"

if [ -z "$1" ]
then
    hdfs_dataset_path=$DEFAULT_HDFS_DATASET_PATH
    echo "First argument null => using default hdfs_dataset_path=$DEFAULT_HDFS_DATASET_PATH"
else
    hdfs_dataset_path=$1
fi

if [ -z "$2" ]
then
    namenode_dataset_path=$DEFAULT_NAMENODE_DATASET_PATH
    echo "Second argument null => using default namenode_dataset_path=$DEFAULT_NAMENODE_DATASET_PATH"
else
    hdfs_dataset_path=$2
fi

echo "Creating directory on HDFS $hdfs_dataset_path"

hdfs dfs -mkdir -p $hdfs_dataset_path

echo "Copying from namenode $namenode_dataset_path to $hdfs_dataset_path"

hdfs dfs -copyFromLocal $namenode_dataset_path $hdfs_dataset_path

echo "Done"