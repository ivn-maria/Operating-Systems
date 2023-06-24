#!/bin/bash

if [[ $# -eq 0 ]]; then
  echo "At least on argument is expected"
  exit 1
fi

if [[ ! -d "${BACKUP_DIR}" ]]; then
    mkdir -p "${BACKUP_DIR}"
fi

if [[ "${1}" == "-r" ]]; then
    shift
    for file in "$@"; do
        if [[ -d "${file}" ]]; then
            if [[ "$(find "${file}" -mindepth 1 -printf '%f\n')" ]]; then
            tar -czf "${file}_$(date +"%Y-%m-%d-%H-%M-%S").tgz" "${file}"
            mv "${file}_$(date +"%Y-%m-%d-%H-%M-%S").tgz" "${BACKUP_DIR}"
            rm -rf "${file}"
            echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed directory recursively ${file}"
        else
            gzip -c "${file}" > "${file}_$(date +"%Y-%m-%d-%H-%M-%S").gz"
            mv "${file}_$(date +"%Y-%m-%d-%H-%M-%S").gz" "${BACKUP_DIR}"
            rm -rf "${file}"
            echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed directory ${file}"
            fi
     elif [[ -f "${file}" ]]; then
            gzip -c "${file}" > "${file}_$(date +"%Y-%m-%d-%H-%M-%S").gz"
            mv "${file}_$(date +"%Y-%m-%d-%H-%M-%S").gz" "${BACKUP_DIR}"
            rm -f "${file}"
            echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed file ${file}"
        else
            echo "Incorrect arguments"
      fi
    done
else
    for file in "$@"; do
        if [[ -d "${file}" ]]; then
            if [ "$(find "${file}" -mindepth 1 -printf '%f\n')" ]; then
                echo "[$(date +"%Y-%m-%d %H:%M:%S")] Can't remove directory ${file}"
            else
                gzip -c "$file" > "${file}_$(date +"%Y-%m-%d-%H-%M-%S").gz"
                mv "${file}_$(date +"%Y-%m-%d-%H-%M-%S").gz" "${BACKUP_DIR}"
                rm -rf "${file}"
                echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed directory ${file}"
            fi
        elif [[ -f "${file}" ]]; then
            gzip -c "${file}" > "${file}_$(date +"%Y-%m-%d-%H-%M-%S").gz"
            mv "${file}_$(date +"%Y-%m-%d-%H-%M-%S").gz" "${BACKUP_DIR}"
            rm -f "${file}"
            echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed file ${file}"
        else
            echo "Incorrect arguments"
        fi
    done
fi
