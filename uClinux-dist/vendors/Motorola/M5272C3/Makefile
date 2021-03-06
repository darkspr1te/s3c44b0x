#
#	Makefile -- Build instructions for Motorola/M5272C3
#

ROMFSIMG = $(IMAGEDIR)/romfs.img
IMAGE    = $(IMAGEDIR)/image.bin
ELFIMAGE = $(IMAGEDIR)/image.elf

DIRS = 

ROMFS_DIRS = bin dev etc etc/config etc/default home lib mnt proc usr var \
             home/httpd home/httpd/cgi-bin

DEVICES = \
	tty,c,5,0	console,c,5,1	cua0,c,5,64	cua1,c,5,65 \
	mem,c,1,1	kmem,c,1,2	null,c,1,3	zero,c,1,5 \
	\
	random,c,1,8	urandom,c,1,9 \
	\
	ram0,b,1,0	ram1,b,1,1	ram2,b,1,2	ram3,b,1,3 \
	\
	ptyp0,c,2,0	ptyp1,c,2,1	ptyp2,c,2,2	ptyp3,c,2,3 \
	ptyp4,c,2,4	ptyp5,c,2,5	ptyp6,c,2,6	ptyp7,c,2,7 \
	ptyp8,c,2,8	ptyp9,c,2,9	ptypa,c,2,10	ptypb,c,2,11 \
	ptypc,c,2,12	ptypd,c,2,13	ptype,c,2,14	ptypf,c,2,15 \
	\
	ttyp0,c,3,0	ttyp1,c,3,1	ttyp2,c,3,2	ttyp3,c,3,3 \
	ttyp4,c,3,4	ttyp5,c,3,5	ttyp6,c,3,6	ttyp7,c,3,7 \
	ttyp8,c,3,8	ttyp9,c,3,9	ttypa,c,3,10	ttypb,c,3,11 \
	ttypc,c,3,12	ttypd,c,3,13	ttype,c,3,14	ttypf,c,3,15 \
	tty0,c,4,0	tty1,c,4,1	tty2,c,4,2	tty3,c,4,3 \
	ttyS0,c,4,64	ttyS1,c,4,65 \
	\
	rom0,b,31,0	rom1,b,31,1	rom2,b,31,2	rom3,b,31,3 \
	rom4,b,31,4	rom5,b,31,5	rom6,b,31,6	rom7,b,31,7 \
	rom8,b,31,8	rom9,b,31,9  \
	\
	qspi0,c,126,0	qspi1,c,126,1	qspi2,c,126,2	qspi3,c,126,3 \
	qspi4,c,126,4	qspi5,c,126,5	qspi6,c,126,6	qspi7,c,126,7 \
	qspi8,c,126,8	qspi9,c,126,9	qspi10,c,126,10	qspi11,c,126,11	\
	qspi12,c,126,12	qspi13,c,126,13	qspi14,c,126,14 \
	\
	ipsec,c,36,10 \
	ppp,c,108,0 \
	\
	usb0,c,127,0	usb1,c,127,1	usb2,c,127,2	usb3,c,127,3  \
	usb4,c,127,4	usb5,c,127,5	usb6,c,127,6	usb7,c,127,7

all:
	dirs=$(DIRS) ; \
	for i in $$dirs ; do  make -C $$i || exit $? ; done

clean:
	-dirs=$(DIRS) ; \
	for i in $$dirs; do [ ! -d $$i ] || make -C $$i clean; done

romfs:
	[ -d $(ROMFSDIR)/$$i ] || mkdir -p $(ROMFSDIR)
	for i in $(ROMFS_DIRS); do \
		[ -d $(ROMFSDIR)/$$i ] || mkdir -p $(ROMFSDIR)/$$i; \
	done
	for i in $(DEVICES); do \
		touch $(ROMFSDIR)/dev/@$$i; \
	done
	$(ROMFSINST) -s /var/tmp /tmp
	$(ROMFSINST) ../../Generic/romfs /
	$(ROMFSINST) ../../Generic/httpd /home/httpd
	$(ROMFSINST) ../../Generic/big/inittab /etc/inittab
	$(ROMFSINST) ../../Generic/big/rc /etc/rc
	if [ $(CONFIG_BLK_DEV_INITRD) = "y" ]; then \
		ln -sf bin/init $(ROMFSDIR)/linuxrc; \
	fi
	echo "$(VERSIONSTR) -- " `date` > $(ROMFSDIR)/etc/version

image:
	[ -d $(IMAGEDIR) ] || mkdir -p $(IMAGEDIR)
	genromfs -v -V "ROMdisk" -f $(ROMFSIMG) -d $(ROMFSDIR)
	m68k-elf-objcopy -O binary $(ROOTDIR)/$(LINUXDIR)/linux \
	  $(IMAGEDIR)/linux.bin
	cat $(IMAGEDIR)/linux.bin $(ROMFSIMG) > $(IMAGE)
	$(ROOTDIR)/tools/cksum -b -o 2 $(IMAGE) >> $(IMAGE)
	[ -n "$(NO_BUILD_INTO_TFTPBOOT)" ] || cp $(IMAGE) /tftpboot
	BSS=`m68k-elf-objdump --headers $(ROOTDIR)/$(LINUXDIR)/linux | \
	  grep .bss` ; \
	ADDR=`set -- $${BSS} ; echo 0x$${4}` ; \
	m68k-elf-objcopy --add-section=.romfs=$(ROMFSIMG) \
	  --adjust-section-vma=.romfs=$${ADDR} --no-adjust-warnings \
	  --set-section-flags=.romfs=alloc,load,data   \
	  $(ROOTDIR)/$(LINUXDIR)/linux $(ELFIMAGE) 2> /dev/null

