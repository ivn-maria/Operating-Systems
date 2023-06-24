#!/bin/bash

if [[ $# -lt 1 ]] || [[ $# -gt 2 ]]; then
  echo "One or two arguments are expected"
  exit 1
fi

if [[ $# -eq 2 ]]; then
  find "${1}" -type f -links +${2} -printf "%p\n"
else find "${1}" -type l ! -exec test -e {} \; -printf "%p\n"
fi
