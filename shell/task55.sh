#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "One argument needed"
    exit 1
fi

dir=${1}
log="logfile"

if [[ ! -f ${log} ]]; then
    touch ${log}
fi

toChange="$(mktemp)"

if [[ "$(stat -c "%s" ${log})" -eq 0 ]]; then
    find ${dir} -type f | egrep '[^_]+_report-[[:digit:]]+.tgz$' >${toChange}
else
    while read line; do
        if ! cat ${log} | grep -q "${line}$"; then
            echo "${line}" >> ${toChange}
        elif [[ "$(cat $log | grep "${line}$")" != "$(sha256sum ${line})" ]]; then
            echo "${line}" >> ${toChange}
        fi
    done < <(find ${dir} -type f | egrep '^[^_]+_report-[[:digit]]+.tgz$')
fi

while read line; do
    base="$(basename ${line})”
    name="$(echo ${base} | cut -d '_' -f 1)"
    time="$(echo ${base} | cut -d '_' -f 2 | cut -d '-' -f 2 | cut -d '.' -f 1)"
    if tar -tf ${line} | egrep -q "^meow.txt$"; then
        tar -xf ${line} "meow.txt"
        meow_file="$(find . -mindepth 1 -maxdepth 1 -type f -name "meow.txt")"
        if [[ ! -d "./extracted" ]]; then
            mkdir ./extracted
        fi
        mv "${meow_file}" ./extracted/${name}_${time}.txt
    fi
done < <(cat ${toChange})

while read line ;do
    sha256sum ${line} >> ${log}
done < <(find ${dir} -type f | egrep '^[^_]+_report-[[:digit:]]+.tgz$')

rm "${toChange}"
