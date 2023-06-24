#!/bin/bash

for FILE in "$@"; do
    if [[ -f “${FILE}” ]] && [[ -r “${FILE}” ]]; then
        echo "${FILE} is redable"
    elif [[ -d "$FILE" ]] && [[ -r "{FILE}" ]]; then
         echo "${FILE} is redable"
         echo "files with size smaller than the num of files in the dir:"
         for file in $(find ${FILE} -maxdepth 1 -mindepth 1 -type f -size \
             -$(find ${FILE} -maxdepth 1 -mindepth 1 -type f | wc -l)c); do
            echo “${file}”
        done
    else
        echo  "${FILE} is not redable"
    fi  
done
