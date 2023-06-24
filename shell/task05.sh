#!/bin/bash
 
read -p "First file: " file1
read -p "Second file: " file2
read -p "Output file: " outfile
 
mergedfile=$(paste -d "\n" ${file1} ${file2})
sortedfile=$(echo "${mergedfile}"  sort)
echo "${sortedfile}" > ${outfile}
