/*
 * include/asm-arm/arch-s3c24a0/memory.h
 *
 * $Id: memory.h,v 1.1 2004/06/03 01:35:47 hcyun Exp $
 *
 * (C) Heechul Yun <heechul.yun@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_ARCH_MEMORY_H_
#define __ASM_ARCH_MEMORY_H_

#define TASK_SIZE       (0x13000000UL)
#define TASK_SIZE_26    TASK_SIZE

#ifndef CONFIG_DRAM_BASE
#define PHYS_OFFSET (0x10000000UL)
#define END_MEM     (0x13000000UL)
#else
#define PHYS_OFFSET (CONFIG_DRAM_BASE)
#define END_MEM     (CONFIG_DRAM_BASE + CONFIG_DRAM_SIZE)
#endif
#define PAGE_OFFSET (PHYS_OFFSET)


#define __virt_to_phys(vpage) ((unsigned long) (vpage))
#define __phys_to_virt(ppage) ((unsigned long) (ppage))
#define __virt_to_bus(x) __virt_to_phys(x)
#define __bus_to_virt(x) __phys_to_virt(x)


#endif /* __ASM_ARCH_MEMORY_H_ */
