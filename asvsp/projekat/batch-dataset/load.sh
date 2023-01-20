#!/bin/sh

DEFAULT_HDFS_DATASET_PATH="/home/projekat/batch-dataset"
DEFAULT_NAMENODE_DATASET_PATH="/home/projekat/batch-dataset"
DEFAULT_DATASET_01_NAME="output.csv"
DEFAULT_DATASET_02_NAME="uscities.csv"

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

if [ -z "$3" ]
then
    dataset_01_name=$DEFAULT_DATASET_01_NAME
    echo "Third argument null => using default dataset_01_name=$DEFAULT_DATASET_01_NAME"
else
    dataset_01_name=$3
fi

if [ -z "$4" ]
then
    dataset_02_name=$DEFAULT_DATASET_02_NAME
    echo "Fourth argument null => using default dataset_02_name=$DEFAULT_DATASET_02_NAME"
else
    dataset_02_name=$4
fi

FILE="$namenode_dataset_path/$dataset_01_name"
if [ ! -f "$FILE" ]; then
    DEFAULT_FILEID="1RD35ftdjbBOfRUdjCR3LoEhZLaQNb_2c"
    DEFAULT_FILENAME="dataset.tar.gz"
    echo "File '$FILE' doesn't exist."
    echo "Downloading the file"
    "$(pwd)/download_dataset.sh" $DEFAULT_FILEID $DEFAULT_FILENAME $dataset_01_name
fi

echo "Creating directory on HDFS $hdfs_dataset_path"

hdfs dfs -mkdir -p $hdfs_dataset_path

echo "Copying from namenode $FILE to $hdfs_dataset_path"

hdfs dfs -copyFromLocal $FILE $hdfs_dataset_path

FILE="$namenode_dataset_path/$dataset_02_name"
if [ ! -f "$FILE" ]; then
    DEFAULT_FILEID="1kStv5cZTDFrRjqhUFRJw-LrViAWY9hLT"
    DEFAULT_FILENAME="dataset.tar.gz"
    echo "File '$FILE' doesn't exist."
    echo "Downloading the file"
    "$(pwd)/download_dataset.sh" $DEFAULT_FILEID $DEFAULT_FILENAME $dataset_02_name
fi

echo "Copying from namenode $FILE to $hdfs_dataset_path"

hdfs dfs -copyFromLocal $FILE $hdfs_dataset_path

echo "Done"