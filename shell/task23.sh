#!/bin/bash

count=0

while IFS= read -r -d '' dir; do
    for file in "${dir}"/*; do
        if [[ -x "${file}" ]]; then
            ((count++)) 
        fi  
    done
done < <(find "$PATH" -maxdepth 1 -type d -print0)

echo "${count}"
