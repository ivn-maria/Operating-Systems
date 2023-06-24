#!/bin/bash

if [[ $# -lt 1 ]] || [[ $# -gt 2 ]]; then
  echo "One or two arguments are expected"
  exit 1
fi

if [[ $# -eq 2 ]]; then
    OUTPUT_FILE=${2}
else OUTPUT_FILE=/dev/stdout
fi

DIRECTORY=${1}
while read FILE TYPE; do
    if [[ "${TYPE}" == "broken" ]]; then
        (( ${broken_symlinks}++ ))
    else target=$(readlink -f "${DIRECTORY}/${FILE}")
        echo "${FILE} -> ${target}" >> ${OUTPUT_FILE}
    fi  
done < <(find ${DIRECTORY} -type l -printf "%f %Y\n")

echo "Broken symlinks: ${broken_symlinks} " >> ${OUTPUT_FILE}
