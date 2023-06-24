#!/bin/bash

if [[ "${EUID}" -ne 0 ]]; then
    echo "This script must be run as a root"
    exit 1
fi

USER_IDS=$(ps -e -o uid= | sort | uniq)
for USER_ID in ${USER_IDS}; do
    USERNAME=$(id -nu ${USER_ID})
    TOTAL_RSS=$(ps -e -o uid,pid,rss | awk -v user_id=${USER_ID} '$1==user_id {sum+=$3} END {print sum}')
    NUM_PROCESSES=$(ps -e -o uid= | grep $USER_ID | wc -l)
    echo "${USERNAME} has ${NUM_PROCESSES} processes with total rss of ${TOTAL_RSS}"

    AVG_RSS=$((${TOTAL_RSS}/${NUM_PROCESSES}))
    MAX_RSS=$(ps -e -o uid,pid,rss --sort=-rss | awk -v user_id=${USER_ID} '$1==user_id {print $3; exit}')
    if [[ ${MAX_RSS} -gt $((2*${AVG_RSS})) ]]; then
        PID=$(ps -e -o uid,pid,rss --sort=-rss | awk -v user_id=${USER_ID} -v max_rss=${MAX_RSS} '$1==user_id && $3==max_rss {print $2; exit}')
        echo "Killing process ${PID}"
        kill -9 ${PID}
    fi
done
