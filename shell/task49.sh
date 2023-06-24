#!/bin/bash

if [[ "${EUID}" -ne 0 ]]; then
    echo "This script must be run as a root"
    exit 1
fi

if [[ $# -ne 3 ]]; then
  echo "Three arguments are expected"
  exit 1
fi

if [[ -n "$(find ${2} -type f)" ]]; then
  echo "${2} must be empty"
  exit 1
fi

find ${1} -type f -name "*${3}*" | while read file; do
    path=$(echo "${file}" | sed "s|^${1}/||")
    mkdir -p "${2}/$(dirname ${path})"
    mv "${file}" "${2}/${path}"
 done
