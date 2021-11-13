#!/bin/env bash

set -e
export BINUTILSVERSION="2.33.1"
export GCCVERSION="10.1.0"

# Let's get the OS:

if [ -f /etc/os-release ]; then
    # freedesktop.org and systemd
    . /etc/os-release
    OS=$NAME
    VER=$VERSION_ID
elif type lsb_release >/dev/null 2>&1; then
    # linuxbase.org
    OS=$(lsb_release -si)
    VER=$(lsb_release -sr)
elif [ -f /etc/lsb-release ]; then
    # For some versions of Debian/Ubuntu without lsb_release command
    . /etc/lsb-release
    OS=$DISTRIB_ID
    VER=$DISTRIB_RELEASE
elif [ -f /etc/debian_version ]; then
    # Older Debian/Ubuntu/etc.
    OS=Debian
    VER=$(cat /etc/debian_version)
else
    # Fall back to uname, e.g. "Linux <version>", also works for BSD, etc.
    OS=$(uname -s)
    VER=$(uname -r)
fi

echo $OS $VER

if [ "$OS" == "Ubuntu" ] ||[ "$OS" == "Debian" ]; then
	echo -e "\e[1mInstalling depedencies...\e[0m"
	apt-get install make build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo wget gcc binutils

elif [ "$OS" == "Arch Linux" ]; then
	echo "\e[1mInstalling dependencies...\e[0m"
	pacman -Sy base-devel gmp libmpc mpfr wget

else
	echo "\e[31mUnknown Distro\e[0m"
	exit 1
fi

echo -e "\e[1mDownloading Binutils\e[0m"
wget https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILSVERSION.tar.xz

echo -e "\e[1mDownloading GCC\e[0m"
wget ftp://ftp.gnu.org/gnu/gcc/gcc-$GCCVERSION/gcc-$GCCVERSION.tar.xz

echo -e "\e[1mUnpacking..\e[0m"
tar -xvf binutils-$BINUTILSVERSION.tar.xz
tar -xvf gcc-$GCCVERSION.tar.xz
