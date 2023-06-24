#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Two arguments are expected"
    exit 1
fi  

for dir in a b c; do
    if [[ ! -d "${dir}" ]]; then
        mkdir "${dir}"
    fi  
done

for file in *; do
    if [[ -f "${file}" ]]; then
        num_lines=$(wc -l < "${file}")
        if ((${num_lines} < ${1})); then
            mv "${file}" a/
        elif ((${num_lines} < ${2})); then
            mv "${file}" b/
        else mv "${file}" c/
        fi  
    fi  
done
