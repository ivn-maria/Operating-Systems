#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "One argument is expected"
    exit 1
fi

find "${1}" -type l ! -exec test -e {} \; -print
