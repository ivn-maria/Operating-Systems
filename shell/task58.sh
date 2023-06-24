#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Two arguments are expected"
    exit 1
fi  

repo_dir=${1}
package_dir=${2}

package_name=$(basename ${package_dir})
package_version=$(cat ${package_dir}/version)
checksum=$(tar -c ${package_dir}/tree | xz -z - | sha256sum | cut -d ' ' -f 1)

if grep -q "${package_name}-${package_version}" ${repo_dir}/db; then
  old_checksum=$(grep "${package_name}-${package_version}" ${repo_dir}/db | cut -d ' ' -f 2)
  rm ${repo_dir}/packages/${old_checksum}.tar.xz
  sed -i "s/${package_name}-${package_version} ${old_checksum}//" ${repo_dir}/db
fi

cp ${checksum}.tar.xz ${repo_dir}/packages/${checksum}.tar.xz
echo "${package_name}-${package_version} ${checksum}" >> ${repo_dir}/db
sort ${repo_dir}/db -o ${repo_dir}/db
