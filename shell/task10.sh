#!/bin/bash

if [[ $# -ne 1 ]]; then
  echo "One argument expected"
  exit 1
fi

username=$1

while true; do
    if who | grep -q -E "^${username} "; then
    echo "${username} is logged in"
    exit 0
  fi
  sleep 1
done
