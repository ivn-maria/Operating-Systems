#!/bin/bash

if [[ $# -ne 2 ]]; then
  echo "incorrect args"
  exit 1
fi

directory_name=${1}
file_size=${2}

find ${directory_name} -type f -size "+${file_size}c" -print
