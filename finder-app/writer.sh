#!/bin/bash

if [ ! $# -eq 2 ]
then
    echo File name and string should be provided
    exit 1
fi


writefile=$1
writestr=$2

if [ -f $writefile ]
then
    rm $writefile
    echo Directory and the file already exists
    echo "$writestr" > "$writefile"
else
    echo "$writestr" > "$writefile"
fi


