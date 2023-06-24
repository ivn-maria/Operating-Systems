#!/bin/bash

N=10

if [[ "${1}" == "-n" && $# -gt 1 ]]; then
    N=${2}
    shift 2
fi

if [[ $# -eq 0 ]]; then
    echo "More arguments are expected"
    exit 1
fi

for file in $@; do
    id=$(basename "${file}" .log)
    tail -${N} "$f{ile}" | sed -E "s/^([^ ]+) ([^ ]+) (.*)$/\1 \2 ${id} \3/g" 
done | sort -n
