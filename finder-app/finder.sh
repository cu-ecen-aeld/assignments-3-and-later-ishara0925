#!/bin/bash



if [ ! $# -eq 2 ]
then
    echo Directory and search string should be provided
    exit 1
fi

if [ -d "$1" ]
then
    filesdir=$1
else
    echo $1 Does not represent a directory on the filesystem
    exit 1
fi
searchstr=$2

echo The number of files are $(find $filesdir -type f | wc -l) and the number of matching lines are $(grep -r $searchstr $filesdir | wc -l)