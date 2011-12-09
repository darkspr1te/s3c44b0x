/*
 *  linux/include/asm-armnommu/memory.h
 *
 *  Copyright (C) 2000-2002 Russell King
 *  Copyright (C) 2003 Hyok S. Choi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Note: this file should not be included by non-asm/.h files
 */
#ifndef __ASM_ARMNOMMU_MEMORY_H
#define __ASM_ARMNOMMU_MEMORY_H

#include <linux/config.h>
#include <linux/compiler.h>
#include <asm/arch/memory.h>


#ifndef __ASSEMBLY__

#define virt_to_bus(x) ((unsigned long) (x))
#define bus_to_virt(x) ((void *) (x))
#define virt_to_phys(x) ((unsigned long) (x))
#define phys_to_virt(x) ((void *) (x))

#define page_to_phys(page)	(page_to_pfn(page) << PAGE_SHIFT)

#define __virt_to_phys__is_a_macro
#define __phys_to_virt__is_a_macro
#define __virt_to_bus__is_a_macro
#define __bus_to_virt__is_a_macro


#define __pa(x)			__virt_to_phys((unsigned long)(x))
#define __va(x)			((void *)__phys_to_virt((unsigned long)(x)))

/*
 * The DMA mask corresponding to the maximum bus address allocatable
 * using GFP_DMA.  The default here places no restriction on DMA
 * allocations.  This must be the smallest DMA mask in the system,
 * so a successful GFP_DMA allocation will always satisfy this.
 */
#ifndef ISA_DMA_THRESHOLD
#define ISA_DMA_THRESHOLD	(0xffffffffULL)
#endif

#ifndef arch_adjust_zones
#define arch_adjust_zones(node,size,holes) do { } while (0)
#endif

#ifndef CONFIG_DISCONTIGMEM
/*
 * PFNs are used to describe any physical page; this means
 * PFN 0 == physical address 0.
 *
 * This is the PFN of the first RAM page in the kernel
 * direct-mapped view.  We assume this is the first page
 * of RAM in the mem_map as well.
 */
#define PHYS_PFN_OFFSET	(PHYS_OFFSET >> PAGE_SHIFT)

#define page_to_pfn(page)	(((page) - mem_map) + PHYS_PFN_OFFSET)
#define pfn_to_page(pfn)	((mem_map + (pfn)) - PHYS_PFN_OFFSET)

#define virt_to_page(kaddr)	(pfn_to_page(__pa(kaddr) >> PAGE_SHIFT))
#define virt_addr_valid(kaddr)	((unsigned long)(kaddr) >= PAGE_OFFSET && (unsigned long)(kaddr) < (unsigned long)high_memory)

#define VALID_PAGE(page)	((page - mem_map) < max_mapnr)

/* MAP_NR should not be used in 2.4 */
#define MAP_NR(addr)      (((unsigned long) (addr) - PAGE_OFFSET) >> PAGE_SHIFT)

#define PHYS_TO_NID(addr)	(0)

#endif /* CONFIG_DISCONTIGMEM */

/*
 * We should really eliminate virt_to_bus() here - it's deprecated.
 */
#define page_to_bus(page)	(virt_to_bus(page_address(page)))

#endif /* !__ASSEMBLY */
#endif /* __ASM_ARMNOMMU_MEMORY_H */
