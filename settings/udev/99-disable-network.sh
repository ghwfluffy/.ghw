#!/bin/bash

set -eu -o pipefail

IFX="${1}"

ifconfig "${IFX}" down

