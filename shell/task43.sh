#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Two arguments are expected"
    exit 1
fi

SRC=${1}
DST=${2}

mkdir -p "${DST}"
mkdir -p "${DST}/images"
mkdir -p "${DST}/by-date"
mkdir -p "${DST}/by-album"
mkdir -p "${DST}/by-title"

while IFS= read -r -d '' file; do
    title=$(echo "${file}" | sed 's/\.[^.]*$//' | sed -E 's/ *\([^)]+\)//g' | sed -E 's/\s+/\ /g' | sed -E 's / *$//')
    album=$(echo "${file}" | sed -E 's/.*\(([^)]+)\)\.jpg$/\1/g'  | sed -E 's/\s+/ /g' | sed -E 's/ *$//')
    if [[ -z "${album}" ]]; then
        album="misc"
    fi
    date=$(date -r "${file}" +"%Y-%m-%d")
    hash=$(sha256sum "${file}" | cut -d ' ' -f 1 | cut -c -16)

    cp "${file}" "${DST}/images/${hash}.jpg"

    ln -s "../images/${hash}.jpg" "${DST}/by-date/${date}/by-album/${album}/by-title/${title}.jpg"
    ln -s "../images/${hash}.jpg" "${DST}/by-date/${date}/by-title/${title}.jpg"
    ln -s "../images/${hash}.jpg" "${DST}/by-album/${album}/by-date/${date}/by-title/${title}.jpg"
    ln -s "../images/${hash}.jpg" "${DST}/by-album/${album}/by-title/${title}.jpg"
    ln -s "../images/${hash}.jpg" "${DST}/by-title/${title}.jpg"
done < <(find "${SRC}" -type f -name "*.jpg" -print0)
