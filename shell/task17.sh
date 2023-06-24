#!/bin/bash

read -p "String: " STRING

while [[ $# -ne 0 ]]; do
    FILE=${1}
    shift
    echo "File ${FILE}: $(grep -F “${STRING}” ${FILE} | wc -l)"
done
