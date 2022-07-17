#!/bin/bash

GHWUSER="ghw"
if [ -v USER ]; then
    GHWUSER="${USER}"
fi
GHWDIR="/home/${GHWUSER}/.ghw"

if [ "${TERM:0:5}" == "xterm" ]; then
    for file in $(find "${GHWDIR}/bashrc" -type f ! -name '.bashrc' ! -name '*.swp' | sort); do
        source "${file}"
    done
fi
