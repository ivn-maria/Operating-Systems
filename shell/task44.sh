#!/bin/bash

if [[ "${EUID}" -ne 0 ]]; then
    echo “This script must be run as a root"
    exit 1
fi

if [[ $# -ne 1 ]]; then
    echo "One argument is expected"
    exit 1
fi

users=$(cut -d : -f 1 /etc/passwd)

for user in ${users}; do
    processes=$(ps -U ${user} -o pid,rss | tail -n +2)
    total_rss=0
    while read -r pid rss; do
        total_rss=$(( ${total_rss} + rss ))
    done <<< "${processes}"

    if [[ "${total_rss}" -gt "${1}" ]]; then
        echo "Total RSS for ${user} is more than ${1}: ${total_rss}"
        echo "Killing the process with highest RSS"
        while read -r pid rss; do
            if [[ "${rss}" -eq "$(echo "${processes}" | sort -k 2 -n | tail -1 | awk '{print $2}')" ]]; then
                kill -15 "${pid}"
            fi
        done <<< "${processes}"
    fi
done
