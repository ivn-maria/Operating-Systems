#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Two arguments are expected"
    exit 1
fi  

file1=${1}
file2=${2}
count1=$(grep -c $(basename "${file1}") "${file1}")
count2=$(grep -c $(basename "${file2}") "${file2}")

if [[ "${count1}" -gt "${count2}" ]]; then
    winner="${file1}"
else winner="${file2}"
fi  

cat "${winner}" | sed 's/^[0-9]\+г\. [^ ]\+ - //' | sort > "$(basename "${winner}" ".txt").songs"
