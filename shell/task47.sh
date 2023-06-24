#!/bin/bash

if [[ "${EUID}" -ne 0 ]]; then
    echo "This script must be run as a root"
    exit 1
fi

root_rss=$(ps -U root -o rss= | awk '{total += $1} END {print total}')
for user in $(cut -d : -f 1 /etc/passwd | grep -v "root"); do
    if [[ ! -d "$(echo ~${user})" ]] || [[ "$(stat -c '%U' $(echo ~${user}))" != "${user}" ]] || [[ ! -w "$(echo ~${user})" ]]; then
        user_rss=$(ps -U "${user}" -o rss= | awk '{total += $1} END {print total}')
        if [[ ${user_rss} -gt ${root_rss} ]]; then
            echo "User "${user}" is using more memory than root user. Killing all processes..."
            pkill -u ${user}
        fi
    fi
done
