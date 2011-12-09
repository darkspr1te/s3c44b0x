/*
 *  linux/include/asm-armnommu/mmu.h
 *
 *  Copyright (C) 2002, David McCullough <davidm@snapgear.com>
 *  fixed for 2.6 by Hyok S. Choi <hyok.choi@samsung.com>
 */

#ifndef __ARM_MMU_H
#define __ARM_MMU_H


struct mm_rblock_struct {
	int	size;
	int	refcount;
	void	*kblock;
};

struct mm_tblock_struct {
	struct mm_rblock_struct	*rblock;
	struct mm_tblock_struct	*next;
};

typedef struct {
	struct mm_tblock_struct	tblock;
	unsigned long		end_brk;
} mm_context_t;

#if __LINUX_ARM_ARCH__ >= 6
#define ASID(mm)	((mm)->context.id & 255)
#else
#define ASID(mm)	(0)
#endif

#endif
