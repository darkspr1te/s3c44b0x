#
# arch/armnommu/boot/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995-2002 Russell King
# Modified by Hyok S. Choi, 2004
#

MKIMAGE         := $(srctree)/scripts/mkuboot.sh

# Note: the following conditions must always be true:
#   ZRELADDR == virt_to_phys(TEXTADDR)
#   PARAMS_PHYS must be with 4MB of ZRELADDR
#   INITRD_PHYS must be in RAM

   zreladdr-$(CONFIG_ARCH_S5C7375)	:= 0x00008000
initrd_phys-$(CONFIG_ARCH_S5C7375)      := 0x00300000
   zreladdr-$(CONFIG_ARCH_ATMEL)	:= 0x01000000
initrd_phys-$(CONFIG_ARCH_ATMEL)	:= 0x01300000
   zreladdr-$(CONFIG_ARCH_S3C44B0)	:= 0x0c008000
  ztextaddr-$(CONFIG_ARCH_S3C44B0)	:= 0x0c300000

ZRELADDR    := $(zreladdr-y)
ZTEXTADDR   := $(ztextaddr-y)
PARAMS_PHYS := $(params_phys-y)
INITRD_PHYS := $(initrd_phys-y)
#
# We now have a PIC decompressor implementation.  Decompressors running
# from RAM should not define ZTEXTADDR.  Decompressors running directly
# from ROM or Flash must define ZTEXTADDR (preferably via the config)
# FIXME: Previous assignment to ztextaddr-y is lost here. See SHARK
ifeq ($(CONFIG_ZBOOT_ROM),y)
ZTEXTADDR	:= $(CONFIG_ZBOOT_ROM_TEXT)
ZBSSADDR	:= $(CONFIG_ZBOOT_ROM_BSS)
else
ZTEXTADDR	:= 0
ZBSSADDR	:= ALIGN(4)
endif

export	ZTEXTADDR ZBSSADDR ZRELADDR INITRD_PHYS PARAMS_PHYS

targets := Image zImage bootpImage

$(obj)/Image: vmlinux FORCE
	$(call if_changed,objcopy)
	@echo '  Kernel: $@ is ready'

$(obj)/zImage:	$(obj)/compressed/vmlinux FORCE
	$(call if_changed,objcopy)
	@echo '  Kernel: $@ is ready'

quite_cmd_uimage = UIMAGE $@
      cmd_uimage = $(CONFIG_SHELL) $(MKIMAGE) -A arm -O linux -T kernel \
		   -C none -a $(ZRELADDR) -e $(ZRELADDR) \
		   -n 'Linux-$(KERNELRELEASE)' -d $< $@

targets += uImage
$(obj)/uImage:	$(obj)/zImage
	$(call if_changed,uimage)
	@echo '  Image $@ is ready'

$(obj)/bootpImage: $(obj)/bootp/bootp FORCE
	$(call if_changed,objcopy)
	@echo '  Kernel: $@ is ready'

$(obj)/compressed/vmlinux: vmlinux FORCE
	$(Q)$(MAKE) $(build)=$(obj)/compressed $@

$(obj)/bootp/bootp: $(obj)/zImage initrd FORCE
	$(Q)$(MAKE) $(build)=$(obj)/bootp $@

.PHONY: initrd
initrd:
	@test "$(INITRD_PHYS)" != "" || \
	(echo This machine does not support INITRD; exit -1)
	@test "$(INITRD)" != "" || \
	(echo You must specify INITRD; exit -1)

subdir-	    := bootp compressed
