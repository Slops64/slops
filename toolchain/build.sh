#!/bin/env bash

set -e
export BINUTILSVERSION="2.33.1"
export GCCVERSION="10.1.0"

dirname=$(pwd)
result="${dirname%"${dirname##*[!/]}"}" # extglob-free multi-trailing-/ trim
result="${result##*/}"                  # remove everything before the last /
echo "Current directory: $(pwd)"
if [ $result != "toolchain" ]; then
	echo "Entering directory \`toolchain\` "
	cd toolchain
fi

export PREFIX="$(pwd)/cross"
export TARGET=x86_64-elf
export PATH="$PREFIX/bin:$PATH"
mkdir -p cross
rm -r -f *.xz
echo "PREFIX: $PREFIX, TARGET: $TARGET"

if test -r "build-gcc"; then
	rm -r build-gcc
fi

if test -r "build-binutils"; then
	rm -r build-binutils
fi

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

echo "$OS" "$VER"

if [ "$OS" == "Ubuntu" ] ||[ "$OS" == "Debian" ]; then
	echo -e "\e[32mInstalling depedencies...\e[0m"
	apt-get install make build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo wget gcc binutils

elif [ "$OS" == "Arch Linux" ]; then
	echo -e "\e[32mInstalling dependencies...\e[0m"
	pacman -Sy base-devel gmp libmpc mpfr wget

else
	echo "\e[332mUnknown Distro\e[0m"
	exit 1
fi

echo -e "\e[32mDownloading Binutils\e[0m"
wget https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILSVERSION.tar.xz

echo -e "\e[32mDownloading GCC\e[0m"
wget ftp://ftp.gnu.org/gnu/gcc/gcc-$GCCVERSION/gcc-$GCCVERSION.tar.xz

echo -e "\e[32mUnpacking...\e[0m"
tar -xvf binutils-$BINUTILSVERSION.tar.xz
tar -xvf gcc-$GCCVERSION.tar.xz

mkdir build-binutils
mkdir build-gcc
cd build-binutils

echo -e "\e[32mBuilding binutils\e[0m"
../binutils-$BINUTILSVERSION/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
sudo make all -j
sudo make install -j

echo -e "\e[32mBuilding GCC\e[0m"
cd ..
cd build-gcc
echo "MULTILIB_OPTIONS += mno-red-zone" > ../gcc-$GCCVERSION/gcc/config/i386/t-x86_64-elf
echo "MULTILIB_DIRNAMES += no-red-zone" >> ../gcc-$GCCVERSION/gcc/config/i386/t-x86_64-elf

../gcc-$GCCVERSION/configure --prefix="$PREFIX" --target="$TARGET" --with-sysroot --disable-nls --enable-languages=c --with-newlib
sudo make -j all-gcc
sudo make -j all-target-libgcc
sudo make -j install-gcc
sudo make -j install-target-libgcc
echo "Done"
