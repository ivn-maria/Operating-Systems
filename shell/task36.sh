#!/bin/bash

if [[ $# -ne 2 ]]; then
  echo "Two arguments are expected"
  exit 1
fi

src_dir=${1}
dst_dir=${2}

for file in "${src_dir}"/*; do
    if [[ "$(basename "${file}")" == .* ]] && [[ "$(basename "${file}")" == *".swp" ]]; then
        continue
    fi
    if [[ -d "${file}" ]]; then
      mkdir -p "${dst_dir}/$(basename "${file}")"
      ./script.sh "${file}" "${dst_dir}/$(basename "${file}")"
    else
      cp "${file}" "${dst_dir}"
    fi
done
