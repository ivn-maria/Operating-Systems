#!/bin/bash


if [[ ! $# -eq 1 ]] ;then
    echo "One rgument is expected"
    exit 1
fi  

file=${1}
clear_file="$(mktemp)"

while read line; do
    clear_line="$(echo "${line}" | sed -E 's/[[:space:]]+/ /g' | sed -E 's/^ //' | sed -E 's/ $//')"
    if echo "${clear_line}" | egrep -q "^[A-Z0-9]{,4} (disabled|enabled)$"; then
        echo "${clear_line}" >> "${clear_file}"
    elif echo "${clear_line}" | egrep -q "^[A-Z0-9]{,4} (disabled|enabled) #.*$"; then
        echo "${clear_line}" | cut -d "#" -f 1 | sed -E 's/ $//' >> "${clear_file}"
    fi  
done < <(cat ${file})

while read line ;do
    target_device="$(echo "${line}" | cut -d ' ' -f 1)"
    target_status="$(echo "${line}" | cut -d ' ' -f 2)"
    
    if cat /proc/acpi/wakeup | sed -E 's/^[[:space:]]+//' | egrep -q "^${target_device}"; then
        curr_line="$(cat /proc/acpi/wakeup | sed -E 's/^[[:space:]]+//' | egrep "^${target_device}" | sed -E 's/[[:space:]]+/ /g' | sed -E 's/^ //' | sed -E 's/ $//')"
        curr_status="$(echo "${curr_line}" | cut -d ' ' -f 3 | sed -E 's/^\*//')"
        if [[ "${curr_status}" != "${target_status}" ]]; then
            echo "${target_device}" > /proc/acpi/wakeup
        fi  
    else
        echo "${target_device} unavailable"
    fi  
done < <(cat "${clear_file}")

rm "${clear_file}"
