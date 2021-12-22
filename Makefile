include Makefile.config

ifneq ($(V),)
	SILENCE	=
else
	SILENCE = @
endif

ifneq ($(DEBUG),)
  COMMON_CFLAGS     += -g -DDEBUG
else
  COMMON_CFLAGS     += -DNDEBUG -Os
endif

LIMINE_DIR := limine
LIMINE := $(LIMINE_DIR)/limine-install

QEMU := qemu-system-x86_64
QEMU_FLAGS := -M q35 -m 2G -debugcon file:debug.txt # -s -S

TMP_ISO_ROOT := tmp_iso_root

.PHONY: all clean distclean

all: $(ISO_IMAGE)

run: $(ISO_IMAGE)
	$(QEMU) $(QEMU_FLAGS) -cdrom $(ISO_IMAGE)

$(LIMINE_DIR):
	@echo "Downloading Limine..."
	$(SILENCE)git clone https://github.com/limine-bootloader/limine.git --branch=v2.0-branch-binary --depth=1 > /dev/null 2>&1


$(LIMINE): $(LIMINE_DIR)
	@echo "Building Limine..."
	$(SILENCE)$(MAKE) -C $(LIMINE_DIR) > /dev/null 2>&1

$(KERNEL): $(OBJ)
	$(SHOW_LD) $@
	$(SILENCE)$(LD) $(LDFLAGS) $^ -o $@

-include $(HEADER_DEPS)
%.o: %.c
	$(SHOW_CC) $@
	$(SILENCE)$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(SHOW_CC) $@
	$(SILENCE)$(AS) $(ASFLAGS) $< -o $@

$(ISO_IMAGE): $(KERNEL) $(LIMINE)
	$(SHOW_INSTALL) $@
	$(SILENCE)rm -rf $(TMP_ISO_ROOT)
	$(SILENCE)mkdir -p $(TMP_ISO_ROOT)
	$(SILENCE)cp $(KERNEL) \
		limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-eltorito-efi.bin $(TMP_ISO_ROOT)/
	$(SILENCE)xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-eltorito-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(TMP_ISO_ROOT) -o $(ISO_IMAGE) > /dev/null 2>&1
	$(SILENCE)limine/limine-install $(ISO_IMAGE) > /dev/null 2>&1
	$(SILENCE)rm -rf $(TMP_ISO_ROOT)

clean:
	$(SHOW_CLEAN) $(KERNEL)
	$(SILENCE)-rm -f $(ISO_IMAGE) $(OBJ) $(KERNEL) $(HEADER_DEPS)

distclean: clean
	$(SHOW_CLEAN) $(LIMINE_DIR)
	$(SILENCE)-rm -rf $(LIMINE_DIR)
