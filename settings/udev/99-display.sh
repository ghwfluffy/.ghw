#!/bin/bash

set -eu -o pipefail

GHWUSER="fluffy"
GHWDIR="/home/${GHWUSER}/.ghw"

/usr/bin/env su "${GHWUSER}" -c "${GHWDIR}/settings/desktop/display"

