#!/bin/bash

if [[ "$#" -ne 3 ]]; then
  echo "Three arguments are expected"
  exit 1
fi

file1=${1}
file2=${2}
dir=${3}

for cfgfile in $(find "${dir}" -name "*.cfg"); do
 valid=true
 count=1

 while read line; do
    if [[ "${line}" =~ ^(\s*#|$) ]]; then
      continue
    fi

    if [[ ! "${line}" =~ ^\s*{\s*(no-production|volatile|run-all)\s*};\s*$ ]]; then
      echo "Error in ${cfgfile}:"
      echo "Line ${count}:${line}"
      valid=false
    fi

    count=$((${count}+1))
  done < ${cfgfile}

  if [[ "${valid}" = true ]]; then
    cat "${cfgfile}" >> config.cfg

    username=$(basename "${cfgfile}" .cfg) 
    if ! grep -q "^${username}:" "${file1}"; then
      password=$(pwgen 16 1)
      echo "${username}:${password}" >> "${file1}"
      echo "${username} ${password}"
    fi
  fi
done
