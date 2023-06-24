#!/bin/bash

if [[ $# -eq 0 ]]; then
  echo "At least on argument is expected"
  exit 1
fi

if [[ ! -f "${LOG_FILE}" ]]; then
    mkdir -p ~/log; touch "${LOG_FILE}"
fi

if [[ "$1" == "-r" ]]; then
  shift
  for file in "$@"; do
    if [[ -d "${file}" ]]; then
      if [[ "$(find "${file}" -mindepth 1 -printf '%f\n')" ]]; then
        rm -rf "${file}"
        echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed directory recursively ${file}" >> "${LOG_FILE}"
      else
        rm -rf "${file}"
        echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed directory ${file}" >> "${LOG_FILE}"
      fi
    elif [[ -f "${file}" ]]; then
      rm -rf "${file}"
      echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed file ${file}" >> "${LOG_FILE}"
    else
      echo "incorrect args"
    fi
  done
else
  for file in "$@"; do
    if [[ -d "${file}" ]]; then
      if [[ "$(find "${file}" -mindepth 1 -printf '%f\n')" ]]; then
          echo "[$(date +"%Y-%m-%d %H:%M:%S")] Can't remove directory ${file}" >> "${LOG_FILE}"
      else
        rm -rf "${file}"
        echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed directory ${file}" >> "${LOG_FILE}"
      fi
    elif [[ -f "${file}" ]]; then
      rm -rf "${file}"
      echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed file ${file}" >> "${LOG_FILE}"
    else
      echo "Incorrect arguments"
    fi
  done
fi
