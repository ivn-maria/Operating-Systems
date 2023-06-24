#!/bin/bash

if [[ "${EUID}" -ne 0 ]]; then
    echo "This script must be run as a root"
    exit 1
fi

awk -F : '{print $1,$3,$6}' /etc/passwd | while read user uid dir; do
  if [[ ! -d "${dir}" ]]; then
    echo "User ${user} has no home directory (${dir})"
  elif [[ ! -w "${dir}" ]]; then
    echo "User ${user} cannot write to home directory (${dir})"
  fi
done
