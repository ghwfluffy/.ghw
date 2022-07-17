#!/bin/bash

set -eu -o pipefail

SRC="${1}"
DEST="${2}"

ip link set "${SRC}" down
ip link set "${SRC}" name "${DEST}"
ip link set "${DEST}" up

