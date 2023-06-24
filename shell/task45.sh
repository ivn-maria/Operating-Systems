!/bin/bash

if [[ "${EUID}" -ne 0 ]]; then
    echo "This script must be run as a root"
    exit 1
fi

if [[ $# -ne 1 ]]; then
    echo "One argument is expected"
    exit 1
fi

FOO=${1}

echo "Users with more processes than ${FOO}:"
count=$(ps -U "${FOO}" -o user= | sort | uniq -c | awk '{print $1}')
ps -eo user= | sort | uniq -c | awk -v count="${count}" '$1 > count {print $2}'

TOTAL_SECONDS=$(ps -eo time= | awk '{split($1, a, ":"); print a[1]*3600+a[2]*60+a[3]}' | paste -sd+ | bc)
NUM_USERS=$(ps -eo user= | sort | uniq | wc -l)
AVG_TIME=$((${TOTAL_SECONDS}/${NUM_USERS}))
echo "Average time per user: ${AVG_TIME}"

FOO_PIDS=$(ps -U "${FOO}" -o pid=)
for PID in ${FOO_PIDS}; do
  TIME_SECONDS=$(ps -o time= -p ${PID} | awk '{split($1, a, ":"); print a[1]*3600+a[2]*60+a[3]}')
  if [[ -n "${TIME_SECONDS}" ]] && [[ ${TIME_SECONDS} -gt $(( 2*${AVG_TIME} )) ]]; then
    echo "Killing process ${PID}"
    kill -9 ${PID}
  fi
done
