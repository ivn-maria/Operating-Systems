#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Two arguments are expected"
    exit 1
fi

DIRECTORY=${1}
ARCH=${2}
KERNEL=$(find ${DIRECTORY} -mindepth 1 -maxdepth 1 -type f -name 'vmlinuz-*-'"${ARCH}"'')

if [[ -z "${KERNEL}" ]]; then
    echo "No kernel found"
    exit 1
fi

echo "${KERNEL}" | while read FILE; do
    VERSION=$(echo "${FILE}" | sed 's/^.*vmlinuz-\([0-9]*\.[0-9]*\.[0-9]*\)-.*/\1/')
    FILENAME=$(basename "${FILE}")
    echo "${VERSION} ${FILENAME}"
done | sort -rn | head -n 1 | awk '{print $2}'
