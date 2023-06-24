#!/bin/bash

if [[ $# -eq 0 ]] || [[ $# -gt 2 ]]; then
    echo "One or two arguments are expected"
    exit 1
fi

if [[ ! -d "${BACKUP_DIR}" ]]; then
    exit 1
fi

if [[ "${1}" = "-l" ]]; then
  for file in $(ls -1 "${BACKUP_DIR}"); do
      name=$(echo "${file}" | sed -E 's/^([a-z0-9]+)_.+/\1/')
      date_str=$(echo "${file}" | sed -E 's/^([a-z0-9]+)_//' | sed -E 's/.(gz|tgz)$//' | sed -E 's/-/ /g')
      date=$(echo "${date_str}" | sed -E 's/(....) (..) (..) (..) (..) (..)/\1-\2-\3 \4:\5:\6/')
      echo "${name} (${date})"
  done
  exit 0
fi

if [[ $# -eq 1 ]] || [[ $# -eq 2 ]]; then
  file=${1}
  dir=${2:-.}
  backup_files=$(ls "${BACKUP_DIR}/${file}"*.{tgz,gz} 2>/dev/null)
  num_files=$(echo "${backup_files" | wc -l)

  if [[ ${num_files} -eq 0 ]]; then
    echo "File not found"
    exit 1
  elif [[ ${num_files -eq 1 ]]; then
    backup_file=${backup_files}
  else
    echo "Multiple files found in"
    echo "${backup_files}"
    read -p "Choose file (1-${num_files}): " file_num
    backup_file=$(echo "${backup_files}" | sed -n "${file_num}p")
  fi

  if [[ -n "${backup_file}" ]]; then
    if [[ "${backup_file}" == *.tgz ]]; then
      echo "Restoring directory ${file} to ${dir}"
      tar -xzf "${backup_file}" -C "${dir}"
      mv "${dir}/$(basename "${backup_file}" ".tgz")" "${dir}/${file}"
    elif [[ "${backup_file}" == *.gz ]]; then
      echo "Restoring file ${file} to ${dir}"
      gunzip -c "{$backup_file}" > "{$dir}/${file}"
    fi
  fi
fi
