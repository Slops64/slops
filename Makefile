ISO_IMAGE := slops.iso

KERNEL_DIR := kernel
KERNEL := $(KERNEL_DIR)/kernel.elf

LIMINE_DIR := limine
LIMINE := $(LIMINE_DIR)/limine-install

QEMU := qemu-system-x86_64
QEMU_FLAGS := -M q35 -m 2G -debugcon file:debug.txt

TMP_ISO_ROOT := tmp_iso_root

.PHONY: all clean distclean

all: $(ISO_IMAGE)

run: $(ISO_IMAGE)
	$(QEMU) $(QEMU_FLAGS) -cdrom $(ISO_IMAGE)

$(LIMINE):
	make -C $(LIMINE_DIR)

$(KERNEL):
	$(MAKE) -C $(KERNEL_DIR)

$(ISO_IMAGE): $(KERNEL) $(LIMINE)
	rm -rf $(TMP_ISO_ROOT)
	mkdir -p $(TMP_ISO_ROOT)
	cp kernel/kernel.elf \
		limine.cfg bg.bmp limine/limine.sys limine/limine-cd.bin limine/limine-eltorito-efi.bin $(TMP_ISO_ROOT)/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-eltorito-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(TMP_ISO_ROOT) -o $(ISO_IMAGE)
	limine/limine-install $(ISO_IMAGE)
	rm -rf $(TMP_ISO_ROOT)

clean:
	rm -f $(ISO_IMAGE)
	$(MAKE) -C $(KERNEL_DIR) clean

distclean: clean
	rm -rf $(LIMINE_DIR)
	$(MAKE) -C $(KERNEL_DIR) distclean
