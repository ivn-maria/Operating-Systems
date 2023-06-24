#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "Three arguments are expected"
    exit 1
fi  

number=${1}
prefix=${2}
unit=${3}

base_file="base.csv"
prefix_file="prefix.csv"

prefix_value=$(grep ",${prefix}," "${prefix_file}" | cut -d',' -f3)
unit_name=$(grep ",${unit}," "${base_file}" | cut -d',' -f1)
unit_measure=$(grep ",${unit}," "${base_file}" | cut -d',' -f3)
base_number=$(echo "${number} * ${prefix_value}" | bc)

echo "${base_number} ${unit_name} (${unit_measure}, ${unit})"
