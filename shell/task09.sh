#!/bin/bash

if [[ $# -ne 1 ]] && [[ $# -ne 2 ]]; then
  echo "One or two argument expected"
  exit 1
fi

if [[ $# -eq 1 ]]; then
   OUTPUT=${2};
else OUTPUT=$(date -I)
fi

for FILE in $(find ${1} -maxdepth 1 -mindepth 1 -type f -cmin -45); do
    cp ${FILE} ${OUTPUT}
done

tar -cf archive.tar ${OUTPUT}
