#!/bin/bash

# Use dirname to get the path of the elf file rlative to this script.
TOP_DIR="$(dirname ${0})"

ELF="${TOP_DIR}/reva-firmware/Debug/reva-firmware.elf"
GDB_INIT="${TOP_DIR}/gdbinit"

gdb-multiarch "${ELF}" -x "${GDB_INIT}" "${@}"
