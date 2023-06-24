#!/bin/bash
 
temp="$(mktemp)"
cat "${1}" | tr -dc '{}' > "${temp}"
 
count=0
 
while grep -q . "${temp}"; do
    sed -i 's/{}//g' "${temp}"
    count=$((${count}+1))
done
 
rm "${temp}"
echo "The deepest nesting is ${count} levels"
