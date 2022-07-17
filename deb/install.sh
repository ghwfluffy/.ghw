#!/bin/bash

set -eu -o pipefail

if [ $(id -u) -ne 0 ]; then
    echo "Debian package install script must be run as root." 1>&2
    exit 1
fi

DIR="$(dirname "${0}")"

# Install package list
apt install $(cat "${DIR}/packages.txt")

# Upgrade pip3
python3 -m pip install --upgrade pip

# Install static debians
DEBS=($(find "${DIR}" -name '*.deb'))
if [ ${#DEBS[@]} -gt 0 ]; then
    dpkg -i ${DEBS[@]}
fi

