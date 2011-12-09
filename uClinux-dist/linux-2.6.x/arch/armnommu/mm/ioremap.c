/*
 *  linux/arch/armnommu/mm/ioremap.c
 *
 * Re-map IO memory to kernel address space so that we can access it.
 *
 * (C) Copyright 1995 1996 Linus Torvalds
 *
 * Hacked for ARM by Phil Blundell <philb@gnu.org>
 * Hacked to allow all architectures to build, and various cleanups
 * by Russell King
 * Hacked for uClinux/ARM by Hyok S. Choi <hyok.choi@samsung.com>
 *
 */
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

#include <asm/page.h>
#include <asm/pgalloc.h>
#include <asm/io.h>
#include <asm/tlbflush.h>
void *
__ioremap(unsigned long phys_addr, size_t size, unsigned long flags,
	  unsigned long align)
{
	return (void *) (phys_addr);
}

void __iounmap(void *addr)
{
}
