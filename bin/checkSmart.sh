#!/bin/bash
#
# Output some common SMART error value indicators for detected drives
#

set -eu -o pipefail

for drive in $(find /dev -name 'sd[a-z]' -or -name 'nvme[0-9]' | sort); do
    echo "${drive}"
    DATA="$(sudo smartctl -a "${drive}" || true)"
    echo "${DATA}" | grep "in progress\|test remaining" || true
    echo "${DATA}" | grep '^  1\>\|^  5\>\|^\<197\>\|^\<198\>\|^\<187\>\|^\<188\>' || true
done
