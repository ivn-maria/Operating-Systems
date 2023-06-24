#!/bin/bash

lines="$(mktemp)"
result="$(mktemp)"

egrep "^-?[[:digit:]]+$" >> "${lines}"
max="$(cat "${lines}" | awk 'BEGIN {max=0} $0<0 {$0*=-1}; $0>max {max=$0} END {print max}')"
cat "${lines}" | awk -v max="${max}" -v flag=0 '$0<0 {$0*=-1 ; flag=1}; $0==max && flag==1 {print "-"$0}; $0==max && flag==0 {print $0}; {flag = 0}' > ${result}
cat "${result}" | sort -n | uniq
rm "${lines}" "${result}"
