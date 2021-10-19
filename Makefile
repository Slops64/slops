ISO_IMAGE := slops.iso

export SRCTREE		:= $(realpath .)
CORE				:= mm/ fs/
export CORE			:= $(patsubst %/, $(SRCTREE)/%/built-in.o, $(CORE))
export ARCH 		?= x86_64
export ARCH_DIR 	= arch/$(ARCH)
export KERNEL 		= $(ARCH_DIR)/kernel.elf

export CC := gcc
export AS := gcc
export LD := ld

export ASFLAGS := 	-I./include 	\
					-m64 			\
					-c 			\
					-g

export LDFLAGS := 	-Tlinker.ld   	\
			-nostdlib              	\
			-zmax-page-size=0x1000 	\
			-static                	\
			--no-dynamic-linker    	\
			-ztext

export CFLAGS := 	-c 						\
					-m64 					\
					-nostdlib 				\
					-nostdinc 				\
					-nodefaultlibs 			\
					-fno-stack-protector 	\
					-I./include 			\
					-nostartfiles 			\
					-ffreestanding 			\
					-Wall 					\
					-Wextra 				\
					-Wstrict-prototypes 	\
					-O0 					\
					-g 						\
					#-Werror 					

LIMINE_DIR := limine
LIMINE := $(LIMINE_DIR)/limine-install

QEMU := qemu-system-x86_64
QEMU_FLAGS := -M q35 -m 2G -debugcon file:debug.txt

TMP_ISO_ROOT := tmp_iso_root

.PHONY: all clean distclean $(KERNEL)

all: $(ISO_IMAGE)

run: $(ISO_IMAGE)
	$(QEMU) $(QEMU_FLAGS) -cdrom $(ISO_IMAGE)

$(LIMINE_DIR):
	git clone https://github.com/limine-bootloader/limine.git --branch=v2.0-branch-binary --depth=1

$(LIMINE): $(LIMINE_DIR)
	$(MAKE) -C $(LIMINE_DIR)

$(KERNEL): $(CORE)
	$(MAKE) -C $(ARCH_DIR)

$(CORE):
	$(MAKE) -C $(dir $@)

$(ISO_IMAGE): $(KERNEL) $(LIMINE)
	rm -rf $(TMP_ISO_ROOT)
	mkdir -p $(TMP_ISO_ROOT)
	cp $(KERNEL) \
		limine.cfg bg.bmp limine/limine.sys limine/limine-cd.bin limine/limine-eltorito-efi.bin $(TMP_ISO_ROOT)/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-eltorito-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(TMP_ISO_ROOT) -o $(ISO_IMAGE)
	limine/limine-install $(ISO_IMAGE)
	rm -rf $(TMP_ISO_ROOT)

%-clean: %
	$(MAKE) -C $(dir $<) clean

clean: $(patsubst %, %-clean, $(CORE))
	-rm -f $(ISO_IMAGE)
	-rm -f $(CORE)
	-$(MAKE) -C $(ARCH_DIR) clean

distclean: clean
	-rm -rf $(LIMINE_DIR)
	-$(MAKE) -C $(ARCH_DIR) distclean
