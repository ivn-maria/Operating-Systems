#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Two arguments are expected"
    exit 1
fi

echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" >> ${2}

while read FILE; do
    HNAME=$(basename "${FILE}" | sed -E 's/\.log//')
    VALUES=$(cat "${FILE}" | head -n 8 | tail -n 7 | cut -d ':' -f 2 | xargs | tr ' ' ',')
    LICENSE=$(cat "${FILE}" | grep "This platform has" | sed -E 's/^.* (a|an) (.*) license\.$/\2/')
    SERIALNUM=$(cat "${FILE}" | grep "Serial Number" | cut -d ':' -f 2 | xargs)
    ACTKEY=$(cat "${FILE}" | grep "Activation Key" | cut -d ':' -f 2 | xargs)
    echo "${HNAME},${VALUES},${LICENSE},${SERIALNUM},${ACTKEY}" >> $2
done < <(find ${1} -maxdepth 1 -type f -name "*.log")
