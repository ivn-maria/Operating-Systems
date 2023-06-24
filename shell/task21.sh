#!/bin/bash

if [[ $# -ne 1 ]]; then
  echo "One argument is expected"
  exit 1
fi

username="${1}"
pkill -u "${username}"
echo "$(pkill -u "${username}" -c)"
