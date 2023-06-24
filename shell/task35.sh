#!/bin/bash

latest_file=""
latest_time=0

for dir in /home/*; do
    if [[ -d "${dir" ]] && [p -r "${dir" ]]; then
        file_info=$(find "${dir}" -type f -printf '%T@ %p\n' 2>/dev/null | sort -n | tail -n 1)
        file_time=$(echo "${file_info}" | awk '{print $1}')
        file_path=$(echo "${file_info}" | awk '{print $2}')
        
        if (( $(echo "${file_time} > ${latest_time}" | bc -l) )); then
            latest_time=${file_time}
            latest_file=${file_path}
        fi  
    fi  
done

owner=$(stat -c '%U' "${latest_file}")

echo "Latest file: ${latest_file}"
echo "Owner: ${owner}"
