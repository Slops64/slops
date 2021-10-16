KERNEL := kernel.elf

ASSEMBLER ?= yasm
LD ?= ld

ASMFLAGS ?= -I./arch/x86_64/include/
LDFLAGS ?=

INTERNALLDFLAGS :=         \
	-Tlinker.ld            \
	-nostdlib              \
	-zmax-page-size=0x1000 \
	-static                \
	--no-dynamic-linker    \
	-ztext

INTERNALASMFLAGS :=	\
		--arch=x86	\
		--oformat=elf64	\
		--machine=amd64

ASMFILES      := $(shell find ./arch/x86_64/ -type f -name '*.S')
OBJ         := $(ASMFILES:.S=.o)
HEADER_DEPS := $(ASMFILES:.S=.d)

.PHONY: all clean

all: $(KERNEL)

$(KERNEL): $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) $(INTERNALLDFLAGS) -o $@

-include $(HEADER_DEPS)

%.o: %.S
	$(ASSEMBLER) $(ASMFLAGS) $(INTERNALASMFLAGS) $< -o $@

clean:
	rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS)
