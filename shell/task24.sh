#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "One argument is expected"
    exit 1
fi

ps -U ${1} -o vsz=,rss=,comm= --sort=-vsz | awk 'NR>1 {printf "%s %.2f\n", $3,($2/$1)}'
