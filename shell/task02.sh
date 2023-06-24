#!/bin/bash
 
if [[ $# -ne 1 ]]; then
    echo "One argument expected"
    exit 1
fi  
 
if echo "${1}" | grep -q '^[a-z0-9]*$'; then
    echo "Argument is alphanumeric"
else
    echo "Argument is not alphanumeric"
fi 
