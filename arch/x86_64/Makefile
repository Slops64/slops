KERNEL := kernel.elf

ASMSOURCES  := $(shell find . -type f -name '*.S')
CSOURCES    := $(shell find . -type f -name '*.c')
OBJ         := $(ASMSOURCES:.S=.o) $(CSOURCES:.c=.o)

.PHONY: all clean

all: $(KERNEL)

$(KERNEL): $(OBJ) $(CORE)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(KERNEL) $(OBJ)
