#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Two arguments are expected"
    exit 1
fi

temp_c=$(mktemp)
echo "#include <stdint.h>" > "${temp_c}"
echo "const uint16_t arr[] = {" >> "${temp_c}"
xxd -p -c 2 "${1}" | sed -e 's/../0x&u,/g' -e '$s/,$//' >> "${temp_c}"
echo "};" >> "${temp_c}"
echo "const uint32_t arrN = sizeof(arr)/sizeof(arr[0]);" >> "${temp_c}"

output_h="${2}"
echo "#ifndef _OUTPUT_H_" > "${output_h}"
echo "#define _OUTPUT_H_" >> "${output_h}"
cat "${temp_c}" >> "${output_h}"
echo "#endif /* _OUTPUT_H_ */" >> "${output_h}"

rm "${temp_c}"
