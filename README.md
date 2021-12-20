# slops: SLow OPerating System
[![build](https://img.shields.io/github/workflow/status/Slops64/slops/Makefile%20CI?style=flat-square)](https://github.com/Slops64/slops/actions/workflows/makefile.yml)
[![license](https://img.shields.io/github/license/Slops64/slops?color=yellow&style=flat-square)](https://github.com/Slops64/slops/blob/main/LICENSE)\
[![Discord](https://img.shields.io/discord/764159913924100127?color=blue&label=%20Discord&logo=discord&style=flat)](https://discord.gg/Gb7ZxWAYBc)

slops is a simple unix-like operating system, written in assembly and C.
# How to use

## Build iso image

### 1. build the toolchain (optional)

```bash
sudo ./toolchain/build.sh
```

### 2. make use of the toolchain

```bash
source ./toolchain/setup.sh
```

### 3. the actual build

```bash
make
```
And if you want to enable debug:
```bash
make DEBUG=1
```

## Boot the system

```bash
make run
```

## Remove artifacts

```bash
make clean
```

or if you want to remove all generated files run:

```bash
make distclean
```
# License
Slops is licensed under the MIT license

Enjoy, and happy hacking ! :)
