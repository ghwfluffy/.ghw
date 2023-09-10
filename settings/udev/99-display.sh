#!/bin/bash

set -eu -o pipefail

for GHWUSER in $(w | grep xfce | awk '{print $1}' | sort -u); do
    GHWDIR="/home/${GHWUSER}/.ghw"
    /usr/bin/env su "${GHWUSER}" -c "${GHWDIR}/settings/desktop/display" &> /dev/null || true
done
