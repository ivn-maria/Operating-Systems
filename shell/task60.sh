#!/bin/bash

if [[ $# -ne 3 ]]; then
  echo "Three arguments are expected"
  exit 1
fi

config_file=${1}
key=${2}
value=${3}

user=$(whoami)
date=$(date)

if grep -q "^\s*${key}\s*=\s*${value}" "${config_file}"; then
    continue
elif grep -q "^\s*${key}\s*=" "${config_file}"; then
    sed -i "s/^\(\s*${key}\s*=.*\)$/# \1 # edited at ${date} by ${user}\n${key} = ${value} # added at ${date} by ${user}/" "${config_file}"
else
    echo "${key} = ${value} # added at ${date} by ${user}” >> "${config_file}"
fi
