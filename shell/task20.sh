#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Two arguments are expected"
    exit 1
fi

a=${1}
b=${2}
number=$(( (RANDOM % b) + a ))

count=1

while true; do
    read -p "Guess? " guess
    if [[ “${guess}” -eq “${number}” ]]; then
        echo "RIGHT! Guessed ${number} in ${count} tries!"
        break
    fi  
    if [[ “${guess}” -lt “{$number}” ]]; then
        echo "...bigger!"
    else
        echo "...smaller!"
    fi 
    count=$(( ${count} + 1 ))
done
