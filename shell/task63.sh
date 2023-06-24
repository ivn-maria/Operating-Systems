#!/bin/bash

if [[ $# -lt 1 ]]; then
  echo "At least one argument is expected"
  exit 1
fi

current_date=$(date '+%Y%m%d')

for zonefile in "$@"; do
  soa_record=$(grep -E '^.*\s+IN\s+SOA\s+' "${zonefile}")

  if [[ -z "${soa_record}" ]]; then
    continue
  fi

  if [[ $(grep -q -E '^.*\s+IN\s+SOA\s+' "${zonefile}" | awk '{print NF') == 10 ]]; then
    serial=$(awk '/^@.*\s+IN\s+SOA\s+/ {print $7}' "${zonefile}")
  else
    serial=$(awk '/^@.*\s+IN\s+SOA\s+/ {getline; print $1}' "${zonefile}")
  fi

  serial_date=${serial:0:8}

  if [ "${serial_date}" -lt "${current_date}" ]; then
    new_serial="${current_date}"00
 elif [ "${serial_date}" -eq "${current_date}" ]; then
    new_serial=$((${serial:8} + 1)) 
  else
    echo "Serial number in ${zonefile} is in the future"
    continue
  fi
    
  if [[ $(grep -q -E '^.*\s+IN\s+SOA\s+' "${zonefile}" | awk '{print NF}') == 10 ]]; then
    sed -i "s/^\(.*\s+IN\s+SOA\s+\)${serial}/\1${new_serial}/" "${zonefile}"
  else
      sed -i "s/^\(.*\s+IN\s+SOA\s+(\n\s*\)${serial}/\1${new_serial}/" "${zonefile}"
  fi
done
