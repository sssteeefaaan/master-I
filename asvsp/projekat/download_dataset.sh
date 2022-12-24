#!/bin/bash

DEFAULT_FILEID="1RD35ftdjbBOfRUdjCR3LoEhZLaQNb_2c"
DEFAULT_FILENAME="dataset.tar.gz"
DEFAULT_CSV_OUTPUT_FILENAME="output.csv"

if [ -z "$1" ]
then
    fileid=$DEFAULT_FILEID
    echo "First argument null => using default fileid=$DEFAULT_FILEID"
else
    fileid=$1
fi

if [ -z "$2" ]
then
    filename=$DEFAULT_FILENAME
    echo "Second argument null => using default filename=$DEFAULT_FILENAME"
else
    filename=$2
fi

if [ -z "$3" ]
then
    csvoutputfilename=$DEFAULT_CSV_OUTPUT_FILENAME
    echo "Third argument null => using default csvoutputfilename=$DEFAULT_CSV_OUTPUT_FILENAME"
else
    csvoutputfilename=$3
fi

link="https://drive.google.com/uc?export=download&id=${fileid}"

html=`curl -c ./cookie -s -L $link`

echo "Downloading file from $link to $filename"

curl -Lb ./cookie "https://drive.google.com/uc?export=download&`echo ${html}|grep -Po '(confirm=[a-zA-Z0-9\-_]+)'`&id=${fileid}" -o ${filename}

echo "Extracting zip file $filename"

tar -zxvf "$filename"

decomp_dir=${filename%.tar.gz}

echo "Combining files inside $decomp_dir"

python3 "./faostat/combine.py" "$csvoutputfilename" "$(pwd)/$decomp_dir"

echo "Copying combined csv file"

cp "$(pwd)/$decomp_dir/$csvoutputfilename" "./batch-dataset/$csvoutputfilename"

echo "Removing trash"

rm -R $decomp_dir ./cookie $filename

echo "Done!"