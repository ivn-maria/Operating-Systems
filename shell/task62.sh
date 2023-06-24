#!/bin/bash

if [[ "$USER" != "oracle" && "$USER" != "grid" ]]; then
    echo "This script can only be executed by the oracle or grid users"
    exit 1
fi  
    
if [[ -z "$1" ]]; then
    echo "One argument is expected"
    exit 1
fi  
    
minutes=$((${1} * 60))
homes=$(su - "$USER" -c "adrci exec=\"SET BASE diag_dest; SHOW HOMES\"")
dirs=$(echo "${homes}" | grep -E 'crs|tnslsnr|kfod|asm|rdbms')
paths=$(echo "${dirs}" | awk '{print "/u01/app/'$USER'/" $1 "/" $2}')

for path in ${paths; do
    su - "$USER" -c "adrci exec=\"SET BASE diag_dest; SET HOMEPATH $path; PURGE -AGE $minu    tes\"" 
done
