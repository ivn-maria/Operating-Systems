#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Two arguments are expected"
    exit 1
fi

logfile=${1}
dir=${2}

uniq_names=$(awk -F'[: ]+' '{print $1"."$2}' ${logfile} | sort -u)
num=1
for name in ${uniq_names}; do
    formatted_name=$(echo ${name} | sed 's/\./ /g')
    echo "${formatted_name};${num}" >> ${dir}/dict.txt
    num=$((${num}+1))
done

while read -r line; do
    name=$(echo "${line}" | cut -d ";" -f 1)
    num=$(echo "${line}" | cut -d ";" -f 2)
    touch "${dir}/${num}"
    first_name=$(echo ${name} | cut -d " " -f1)
    last_name=$(echo ${name} | cut -d " " -f2)
    grep -i "^${first_name}[[:space:]]*${second_name}" ${logfile} > ${dir}/${num}.txt
done < "${dir}/dict.txt"
