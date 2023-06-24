#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "One argument is expected"
    exit 1
fi

i=0
while read -r line; do
    i=$((i+1))
    title=$(echo "${line}" | sed -E 's/^[0-9]+ г\. - (.+)$/\1/')
    echo "${i}. ${title}"
done < "${1}" | sort -t ' ' -k 2
