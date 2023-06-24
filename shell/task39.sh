#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "One argument is expected"
    exit 1
fi

dir="${1}"
temp=$(mktemp)

while read file; do
    hash=$(sha256sum ${file})
    echo "${hash} $(stat -c %i ${file})" >> ${temp}
done < <(find ${dir} -type f)

output=$(mktemp)
uniques=$(cat ${temp} | cut -d ' ' -f 1 | sort | uniq)

while read hash; do
    countAll=$(grep -F "${hash}" ${temp} | wc -l)
    uniqHash=$(grep -F "${hash}" ${temp} | cut -d ' ' -f 2 | sort | uniq | wc -l)
    if [[ ${countAll} -eq ${uniqHash} ]]; then
        grep -F "${hash}" ${temp} | cut -d ' ' -f 2 | tail -n +2 >> ${output}
    elif [[ ${uniqHash} -eq 1 ]]; then
        grep -F "${hash}" ${temp} | cut -d ' ' -f 2 | head -n 1 >> ${output}
    else 
        while read inode; do
            countInodes=$(grep -F "${inode}" ${temp} | wc -l)
            if [[ ${countInodes} -eq 1 ]]; then
                grep -F "${inode}" ${temp} | cut -d ' ' -f 2 >> ${output}
            else grep -F "${inode}" ${temp} | cut -d ' ' -f 2 | tail -n 1 >> ${output}
            fi
        done < <(grep -F "${hash}" ${temp} | cut -d ' ' -f 3 | sort | uniq)
    fi 
done < <(echo ${uniques})

echo "$(cat ${output} | sort | uniq -d)"

rm ${temp}
rm ${output}
