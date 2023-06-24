#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "One argument is expected"
    exit 1
fi

dir="${1}"
log="$(mktemp)"

while read line; do
    sum=0
    while read file; do
        lines="$(cat ${file} | wc -l)"
        sum="$(echo ${sum}+${lines} | bc)"
    done < <(find ${dir} -type f | awk -F '/' -v friend="${line}" '$4==friend {print}')
    echo "${line} ${sum}" >> "${log}"
done < <(find ${dir} -type f | cut -d '/' -f 4 | sort | uniq)

cat "${log}" | sort -t ' ' -n -k 2 | tail -n 10

rm "${log}"
