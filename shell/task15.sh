#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "One argument is expected"
    exit 1
fi

DIR="${1}"

while read line; do
    rm "$(find "${DIR}" -type f | xargs md5sum | grep "${line}" | tr -s ' ' | cut -d ' ' -f 2 | sort | tail -n +2)"
done < <(find "${DIR}" -type f | xargs md5sum | sort -t ' ' -k 1 | awk '{a[$1]++} END {for(i in a) if (a[i] > 1) print i}')
