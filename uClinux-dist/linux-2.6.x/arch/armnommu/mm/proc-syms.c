/*
 *  linux/arch/armnommu/mm/proc-syms.c
 *
 *  Copyright (C) 2000-2002 Russell King
 *  Modified by Hyok S. Choi, 2004
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/mm.h>

#include <asm/cacheflush.h>
#include <asm/proc-fns.h>
#include <asm/tlbflush.h>

#ifndef MULTI_CPU
EXPORT_SYMBOL(cpu_dcache_clean_area);
#else
EXPORT_SYMBOL(processor);
#endif

#ifndef MULTI_CACHE
EXPORT_SYMBOL_NOVERS(__cpuc_flush_kern_all);
EXPORT_SYMBOL_NOVERS(__cpuc_flush_user_all);
EXPORT_SYMBOL_NOVERS(__cpuc_flush_user_range);
EXPORT_SYMBOL_NOVERS(__cpuc_coherent_kern_range);
EXPORT_SYMBOL_NOVERS(__cpuc_flush_dcache_page);
#else
EXPORT_SYMBOL(cpu_cache);
#endif
