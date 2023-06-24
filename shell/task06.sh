#!/bin/bash
 
read -p "File: " file
read -p "String: " string
 
if grep -q "${string}" "${file}"; then
    echo "string found in file"
    exit_status=$?
else
    echo "string not found in file"   
    exit_status=$?
fi
 
echo "Exit status: ${exit_status}"
