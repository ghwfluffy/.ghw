#!/bin/bash

set -eu -o pipefail

if [ $(id -u) -le 0 ]; then
    echo "Invalid user. Run as your user." 1>&2
    exit 1
fi

# Find this repo's checkout directory
CWD="$(readlink -f "$(dirname "${0}")")"
USER="$(whoami)"

# Get submodules
(
    cd "${CWD}"
    git submodule update --init --recursive
)

# Install debian packages
(
    echo "Install debian packages"
    cd "${CWD}/deb"
    sudo ./install.sh
)

# Setup bashrc
(
    echo "Save default system bashrc"
    if [ -f ~/.bashrc ] && [ ! -L ~/.bashrc ]; then
        mv ~/.bashrc "${CWD}/bashrc/00-default"
    else
        rm -f ~/.bashrc
    fi

    echo "Setup custom bashrc"
    ln -s "${CWD}/bashrc/.bashrc" ~/.bashrc
)

# Setup desktop environment
(
    echo "Setup udev rules"
    sudo cp $(find "${CWD}/settings/udev" -type f -name '*.rules') /etc/udev/rules.d/
    sudo sed -i "sGHWDIR${CWD}g" /etc/udev/rules.d/*
    sudo sed -i "sGHWUSER${USER}g" /etc/udev/rules.d/*

    echo "Setup udev scripts"
    sudo mkdir -p /usr/local/bin/ghw-udev
    sudo cp $(find "${CWD}/settings/udev" -type f -name '*.sh') /usr/local/bin/ghw-udev
    sudo sed -i "sGHWDIR=.*GHWDIR=\"${CWD}\"" /usr/local/bin/ghw-udev/*
    sudo sed -i "sGHWUSER=.*GHWUSER=\"${USER}\"" /usr/local/bin/ghw-udev/*

    echo "Initialize desktop settings"
    for script in $(find "${CWD}/settings/desktop" -type f); do
        ${script}
    done
)

# Setup vimrc
(
    echo "Setup vimrc"
    rm -f ~/.vimrc
    ln -s "${CWD}/settings/vimrc" ~/.vimrc

    echo "Setup root vimrc"
    sudo rm -f /root/.vimrc
    sudo ln -s ~/.vimrc /root/.vimrc
)

# Setup vim syntax
(
    echo "Setup vim syntax"
    mkdir -p ~/.vim/syntax

    for file in $(find "${CWD}/settings/vim-syntax" -type f -name '*.vim'); do
        base="$(basename "${file}")"
        rm -f ~/.vim/syntax/"${base}"
        ln -s "${file}" ~/.vim/syntax/"${base}"
    done
)

# Setup xfce4
(
    echo "Setup xfce4"
    XFCE_CONFDIR=~/.config/xfce4/xfconf/xfce-perchannel-xml

    mkdir -p "${XFCE_CONFDIR}"
    TZONE="$(timedatectl | grep 'Time zone' | cut -d':' -f2 | awk '{print $1}' || readlink /etc/localtime | cut -d'/' -f5- || echo "America/Boise")"

    for file in screensaver panel; do
        file="xfce4-${file}.xml"
        rm -f "${XFCE_CONFDIR}/${file}"
        ln -s "${CWD}/settings/xfce4/${file}" "${XFCE_CONFDIR}/${file}"
    done
    sed -i "sTZONE${TZONE}" "${XFCE_CONFDIR}/xfce4-panel.xml"

    XFCE_PANELDIR=~/.config/xfce4/panel
    rm -rf "${XFCE_PANELDIR}"
    ln -s "${CWD}/settings/xfce4/panel" "${XFCE_PANELDIR}"
)

# Build apps
(
    echo "Building apps"
    cd "${CWD}/src"
    for dir in *; do
        (cd ${dir}; make;)
    done

    echo "Installing apps"
    for dir in *; do
        (cd ${dir}; make install;)
    done

    echo "Cleaning up apps"
    for dir in *; do
        (cd ${dir}; make clean;)
    done
)

# Third party apps
(
    echo "Building 3rd party apps"
    cd "${CWD}/contrib"
    for mk in *.mk; do
        make -f "${mk}"
    done

    echo "Installing 3rd party apps"
    for dir in *; do
        make -f "${mk}" install
    done

    echo "Cleaning up 3rd party apps"
    for dir in *; do
        make -f "${mk}" clean
    done
)

echo "Ghw setup installed successfully."
