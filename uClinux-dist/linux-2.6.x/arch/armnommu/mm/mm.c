/*
 *  linux/arch/armnommu/mm/mm.c
 *
 *  Copyright (C) 1998-2002 Russell King
 *  Copyright (C) 2004 Hyok S. Choi for nommu
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Page table sludge for ARM v3 and v4 processor architectures.
 */
#include <linux/config.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/bootmem.h>
#include <linux/highmem.h>

#include <asm/pgtable.h>
#include <asm/pgalloc.h>
#include <asm/page.h>
#include <asm/rmap.h>
#include <asm/io.h>
#include <asm/setup.h>
#include <asm/tlbflush.h>

#include <asm/mach/map.h>

/*
 * In order to soft-boot, we need to insert a 1:1 mapping in place of
 * the user-mode pages.  This will then ensure that we have predictable
 * results when turning the mmu off
 */
void setup_mm_for_reboot(char mode)
{
	printk("FIXME: setup_mm_for_reboot %s %i\n",__FILE__,__LINE__);

}


static inline void
free_memmap(int node, unsigned long start_pfn, unsigned long end_pfn)
{
	struct page *start_pg, *end_pg;
	unsigned long pg, pgend;

	/*
	 * Convert start_pfn/end_pfn to a struct page pointer.
	 */
	start_pg = pfn_to_page(start_pfn);
	end_pg = pfn_to_page(end_pfn);

	/*
	 * Convert to physical addresses, and
	 * round start upwards and end downwards.
	 */
	pg = PAGE_ALIGN(__pa(start_pg));
	pgend = __pa(end_pg) & PAGE_MASK;

	/*
	 * If there are free pages between these,
	 * free the section of the memmap array.
	 */
	if (pg < pgend)
		free_bootmem_node(NODE_DATA(node), pg, pgend - pg);
}

static inline void free_unused_memmap_node(int node, struct meminfo *mi)
{
	unsigned long bank_start, prev_bank_end = 0;
	unsigned int i;

	/*
	 * [FIXME] This relies on each bank being in address order.  This
	 * may not be the case, especially if the user has provided the
	 * information on the command line.
	 */
	for (i = 0; i < mi->nr_banks; i++) {
		if (mi->bank[i].size == 0 || mi->bank[i].node != node)
			continue;

		bank_start = mi->bank[i].start >> PAGE_SHIFT;
		if (bank_start < prev_bank_end) {
			printk(KERN_ERR "MEM: unordered memory banks.  "
				"Not freeing memmap.\n");
			break;
		}

		/*
		 * If we had a previous bank, and there is a space
		 * between the current bank and the previous, free it.
		 */
		if (prev_bank_end && prev_bank_end != bank_start)
			free_memmap(node, prev_bank_end, bank_start);

		prev_bank_end = PAGE_ALIGN(mi->bank[i].start +
					   mi->bank[i].size) >> PAGE_SHIFT;
	}
}

/*
 * The mem_map array can get very big.  Free
 * the unused area of the memory map.
 */
void __init create_memmap_holes(struct meminfo *mi)
{
	int node;

	for (node = 0; node < numnodes; node++)
		free_unused_memmap_node(node, mi);
}
