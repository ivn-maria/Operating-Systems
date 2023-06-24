#/bin/bash

lines="$(mktemp)"

egrep "^-?[[:digit:]]+$" >> "${lines}"
max=0
while read line; do
    if [[ ${line} -lt 0 ]]; then
        line=$((${line}*-1))
    fi
    value="$(echo "${line}" | sed -E 's/([[:digit:]])/\1\n/g' | head -n -1 | paste -sd + | bc)"
    if [[ ${max} -lt ${value} ]]; then
        max=${value}
    fi
done < <(cat ${lines})

result="$(mktemp)"
while read line ;do
    flag=0
    if [[ ${line} -lt 0 ]]; then
        line=$((${line}*-1))
        flag=1
    fi
    value="$(echo "${line}" | sed -E 's/([[:digit:]])/\1\n/g' | head -n -1 | paste -sd + | bc)"
    if [[ ${max} -eq ${value} && ${flag} -eq 1 ]]; then
        echo "-${line}" >> ${result}
    elif [[ ${max} -eq ${value} && ${flag} -eq 0 ]] ;then
        echo "${line}" >> ${result}
    fi
done < <(cat ${lines})

echo "$(cat "${result}" | sort -n | head -n 1)"

rm "${lines}" "${result}"
