#!/bin/bash

build_mvp() {
  TOP_LEVEL_DIR=${1}
  if [ -d "mvp-firmware/Debug" ]; then
    echo "Debug build folder for mvp-firmware exists"
    (cd ${TOP_LEVEL_DIR} && cd mvp-firmware/Debug && make -j all)
  fi
}

build_reva() {
  TOP_LEVEL_DIR=${1}
  if [ -d "reva-firmware/Debug" ]; then
    echo "Debug build folder for reva-firmware exists"
    (cd ${TOP_LEVEL_DIR} && cd reva-firmware/Debug/ && make -j all)
  fi
}

move_build_artifacts() {
  cd ${1} && mkdir ${2}
  if [ -d "reva-firmware/Debug" ]; then
    echo "Debug artifact folder for rev-A board found"
    artifact_name="reva-firmware"
    output_folder="${2}/${artifact_name}"
    artifact_location=${artifact_name}/Debug/${artifact_name}
    mkdir ${output_folder} && cp ${artifact_location}.elf ${artifact_location}.map ${artifact_location}.list ${output_folder}
  fi

  if [ -d "mvp-firmware/Debug" ]; then
    echo "Debug artifact folder for MVP board found"
    artifact_name="mvp-firmware"
    output_folder="${2}/${artifact_name}"
    artifact_location=${artifact_name}/Debug/${artifact_name}
    mkdir ${output_folder} && cp ${artifact_location}.elf ${artifact_location}.map ${artifact_location}.list ${output_folder}
  fi
}
