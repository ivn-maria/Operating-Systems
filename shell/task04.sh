#!/bin/bash
 
read -p "Directory path: " dir
 
num_files=$(find ${dir} -type f | wc -l)
echo "Number of file: ${num_files}"
   
num_dirs=$(find ${dir} -mindepth 1 -type d | wc -l)
echo "Number of directories: ${num_dirs}"
