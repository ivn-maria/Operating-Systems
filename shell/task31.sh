#!/bin/bash

count=0

while read line; do
    if [[ ${line} =~ [0-9]*[02468][0-9]* && ! $line =~ [a-w] ]]; then
        ((count ++))
    fi
done < philip-j-fry.txt

echo "The count of searched files is $count"
