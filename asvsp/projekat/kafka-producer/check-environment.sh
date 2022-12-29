#!/bin/sh

if [ -z "$DATASET_API_LINK" ]
then
    echo "Environment variable DATASET_API_LINK not set!"
fi

if [ -z "$KAFKA_BROKER" ]
then
    echo "Environment variable KAFKA_BROKER not set!"
fi

if [ -z "$KAFKA_TOPIC" ]
then
    echo "Environment variable KAFKA_TOPIC not set!"
fi