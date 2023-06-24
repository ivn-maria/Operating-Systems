#!/bin/bash

if [[ $# -ne 1 ]] && [[ $# -ne 2]]; then
    echo "One or two argument expected"
    exit 1
fi  

if [[ $# -eq 2 ]]; then
    DELIMITER=${2} 
else DELIMITER=' '
fi  

NUM=${1}
OUTPUT=''

if [[ ${NUM} -lt 1000 ]]; then
    echo ${NUM}
    exit 0
fi

while [[ ${NUM} -gt 999 ]]; do
    OUTPUT=$(echo "${NUM}" | tail -c4)${DELIMITER}${OUTPUT}
    NUM=$(( ${NUM} / 1000 ))
done

OUTPUT=${NUM}${DELIMITER}${OUTPUT}

if [[ "${DELIMITER}" != ' ' ]]; then
    OUTPUT=$(echo "${OUTPUT}" | sed 's/.$//')
fi

echo "${OUTPUT}"
