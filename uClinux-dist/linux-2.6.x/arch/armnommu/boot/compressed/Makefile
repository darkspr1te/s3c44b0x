#
# linux/arch/armnommu/boot/compressed/Makefile
#
# create a compressed vmlinuz image from the original vmlinux
#
# Note! ZTEXTADDR, ZBSSADDR and ZRELADDR are now exported
# from arch/armnommu/boot/Makefile
#

HEAD	= head.o
OBJS	= misc.o

#
# Architecture dependencies
#
ifeq ($(CONFIG_ARCH_S5C7375),y)
OBJS		+= head-s5c7375.o
endif
ifeq ($(CONFIG_ARCH_S3C44B0),y)
HEAD		= head-s3c44b0.o
endif


SEDFLAGS	= s/TEXT_START/$(ZTEXTADDR)/;s/LOAD_ADDR/$(ZRELADDR)/;s/BSS_START/$(ZBSSADDR)/

targets       := vmlinux vmlinux.lds piggy piggy.gz piggy.o \
                 font.o $(HEAD) $(OBJS)
EXTRA_CFLAGS  := $(CFLAGS_BOOT) -fpic
EXTRA_AFLAGS  := -traditional

LDFLAGS_vmlinux := -p -X \
	$(shell $(CC) $(CFLAGS) --print-libgcc-file-name) -T

$(obj)/vmlinux: $(obj)/vmlinux.lds $(obj)/$(HEAD) $(obj)/piggy.o \
	 	$(addprefix $(obj)/, $(OBJS)) FORCE
	$(call if_changed,ld)
	@:


$(obj)/piggy: vmlinux FORCE
	$(call if_changed,objcopy)

$(obj)/piggy.gz: $(obj)/piggy FORCE
	$(call if_changed,gzip)

LDFLAGS_piggy.o := -r -b binary
$(obj)/piggy.o:  $(obj)/piggy.gz FORCE
	$(call if_changed,ld)

$(obj)/vmlinux.lds: $(obj)/vmlinux.lds.in Makefile arch/armnommu/boot/Makefile .config
	@sed "$(SEDFLAGS)" < $< > $@

$(obj)/misc.o: $(obj)/misc.c include/asm/arch/uncompress.h lib/inflate.c

