#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "One argument is expected"
    exit 1
fi

LOGFILE=${1}
SITES=$(awk '{print $2}' ${LOGFILE} | sort | uniq)
TMPFILE=$(mktemp)

for SITE in ${SITES}; do
  COUNT=$((${HTTP2_COUNT} + ${NONHTTP2_COUNT}))
  echo "${SITE} ${COUNT}" >> ${TMPFILE}
done

TOP_SITES=$(sort -rn -t ' ' -k 2  ${TMPFILE} | head -n 3 | awk '{print $1}')

for SITE in ${TOP_SITES}; do
    HTTP2=$(grep "${SITE}" ${LOGFILE} | grep "HTTP/2.0" | wc -l)
    NONHTTP2=$(grep "${SITE}" ${LOGFILE} | grep -v "HTTP/2.0" | wc -l)
    echo "${SITE} HTTP/2.0:${HTTP2} non-HTTP/2.0:${NONHTTP2}"
    grep "${SITE}" ${LOGFILE} | awk '$8 > 302 {print $1}' | sort | uniq -c | sort -rn | head -n 5
done
