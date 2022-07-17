#!/bin/bash

set -eu -o pipefail

DIR="$(dirname "${0}")"
FILES=($(find "${DIR}" -name "*.txt"))

if [ ${#FILES[@]} -gt 0 ]; then
    cat "${FILES[$(rand ${#FILES[@]})]}"
fi

