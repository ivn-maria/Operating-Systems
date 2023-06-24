#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "One argument is expected"
    exit 1
fi

red='\033[0;31m'
green='\033[0;32m'
blue='\033[0;34m'
reset='\033[0m'

if [[ ${1} == "-r" ]]; then
  color=${red}
elif [[ ${1} == "-g" ]]; then
  color=${green}
elif [[ ${1} == "-b" ]]; then
  color=${blue}
elif [[ ${1} == "-x" ]]; then
  color=""
else
  echo "Incorrect argument"
  exit 1
fi

counter=0

while read line; do
  if [[ "${color}" ]]; then
    echo -e "${color}${line}${reset}"
  else
    echo ${line}
    continue  
  fi
  if [[ ${counter} == 0 ]]; then
    color=$green
  elif [[ ${counter} == 1 ]]; then
    color=$blue
  elif [[ ${counter} == 2 ]]; then
    color=$red
  fi
  counter=$(( (${counter} + 1) % 3 ))
done 
