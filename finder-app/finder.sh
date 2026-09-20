#!/bin/bash

filesdir="$1"
searchstr="$2"

# Verify args provided
if [ "$#" -ne 2 ]; then
	echo "ERROR: Two arguments are required!"
	exit 1
fi

# check filesdir is actual directory
if [ ! -d "$filesdir" ]; then
	echo "ERROR: $filesdir is not a directory!"
	exit 1
fi

# count the number of files in directory
numfiles=$(find "$filesdir" -type f | wc -l)

# count matching lines
numlines=$(grep -r "$searchstr" "$filesdir" | wc -l)

# print
echo "The number of files are $numfiles and the number of matching lines are $numlines"
