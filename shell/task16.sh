#!/bin/bash

if [[ $# -ne 1 ]] && [[ $# -ne 2 ]]; then
    echo "One or two argument is expected"
    exit
fi

if [ "${2}" == "-a" ]; then
    files=$(find ${1} -type f -name '.*' -o -print)
else
    files=$(find ${1} -maxdepth 1 -mindepth 1)
fi

for FILE in ${files} ; do
    if [[ -d ${FILE} ]] ; then
        entries=$(find ${FILE} -mindepth 1 -maxdepth 1 | wc -l)
        echo "${FILE} (${entries} entries)"
    else
        bytes=$(stat -c="%s" ${FILE})
        echo "${FILE} (${bytes} bytes)"
    fi
done
