#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "One argument is expected"
    exit 1
fi

file=/proc/acpi/wakeup
line=$(cat "${file}" | grep "${1}")

if echo "${line}" | grep -q "*enable"; then
    echo "${1}" > ${file} 
fi
