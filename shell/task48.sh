#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "Three arguments are expected"
    exit 1
fi

file=${1}
str1=${2}
str2=${3}
line1=$(grep "^${str1}=" "$file")
line2=$(grep "^${str2}=" "$file")

if [[ -z "${line2}" ]];then
    echo "${str2} not found in the ${file}"
    exit 0
fi

terms1=$(echo "${line1}" | cut -d '=' -f 2 | tr ' ' '\n')
terms2=$(echo "${line2}" | cut -d '=' -f 2 | tr ' ' '\n')
tmpfile1=$(mktemp)
tmpfile2=$(mktemp)
echo "${terms1}" | sort | uniq > "${tmpfile1}"
echo "${terms2}" | sort | uniq > "${tmpfile2}"

terms=$(comm -1 -3 "${tmpfile1}" "${tmpfile2}" | tr '\n' ' ')
sed -i "s/^${str2}=.*$/${str2}=${terms}/" "${file}"

rm "${tmpfile1}" "${tmpfile2}"
