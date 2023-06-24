#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Three arguments expected"
    exit 1
fi

address_file=$1
full_name="$2"
nickname="$3"

matches=$(awk -F':' -v name="${full_name}" '$5==name {print $1}' /etc/passwd)

if [[ -z "${matches}" ]]; then
    echo "No matches found"
    exit 1
fi

if [[ $(echo "${matches}" | wc -l) -gt 1 ]]; then
    echo "Multiple matches found:"
    echo “${matches}” | cat -n
    echo "Choose a match:"
    read choice
    if [[ "${choice}" -eq 0 ]]; then
        exit 1
    fi  
    username=$(echo "${matches}" | sed -n "${choice}p")
else
    username="{$matches}"
fi

echo "${nickname} ${username}" >> "${address_file}"
