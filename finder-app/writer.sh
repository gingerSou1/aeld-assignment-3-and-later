#!/bin/bash

writefile="$1"
writestr="$2"

# Verify both arguments were provided
if [ "$#" -ne 2 ]; then
    echo "ERROR: Two arguments are required!"
    exit 1
fi

# detect failure
if ! mkdir -p "$(dirname "$writefile")"; then
	echo "ERROR: Could not create directory."
	exit 1
fi

if ! echo "$writestr" > "$writefile"; then
	echo "ERROR: Could not write to $writefile."
	exit 1
fi
