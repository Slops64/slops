# slops: SLow OPerating System

slops is a simple unix-like operating system, written in assembly and C.

## BUILD ISO IMAGE

### 1. sync limine

```bash
git submodule update --init
```

### 2. the actual build

```bash
make
```

## BOOT THE SYSTEM

```bash
make run
```

## REMOVE ARTIFACTS

```bash
make clean
```

or if you want to remove all generated files run:

```bash
make distclean
```

Enjoy, and happy hacking ! :)
