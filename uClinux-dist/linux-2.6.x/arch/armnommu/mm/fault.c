/*
 *  linux/arch/armnommu/mm/fault.c
 *
 *  Copyright (C) 1995  Linus Torvalds
 *  Modifications for ARM processor (c) 1995-2002 Russell King
 *  Modifications for nommu or non-paged, Hyok S. Choi, 2003
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/mm.h>
#include <linux/bitops.h>
#include <linux/vmalloc.h>
#include <linux/init.h>

#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/pgalloc.h>
#include <asm/pgtable.h>
#include <asm/tlbflush.h>

#include "fault.h"

/*
 * Some section permission faults need to be handled gracefully.
 * They can happen due to a __{get,put}_user during an oops.
 */
static int
do_sect_fault(unsigned long addr, unsigned int fsr, struct pt_regs *regs)
{
	struct task_struct *tsk = current;
	do_bad_area(tsk, tsk->active_mm, addr, fsr, regs);
	return 0;
}

/*
 * This abort handler always returns "fault".
 */
static int
do_bad(unsigned long addr, unsigned int fsr, struct pt_regs *regs)
{
	return 1;
}

static struct fsr_info {
	int	(*fn)(unsigned long addr, unsigned int fsr, struct pt_regs *regs);
	int	sig;
	const char *name;
} fsr_info[] = {
	/*
	 * The following are the standard ARMv3 and ARMv4 aborts.  ARMv5
	 * defines these to be "precise" aborts.
	 */
	{ do_bad,		SIGSEGV, "vector exception"		   },
	{ do_bad,		SIGILL,	 "alignment exception"		   },
	{ do_bad,		SIGKILL, "terminal exception"		   },
	{ do_bad,		SIGILL,	 "alignment exception"		   },
	{ do_bad,		SIGBUS,	 "external abort on linefetch"	   },
	{ do_translation_fault,	SIGSEGV, "section translation fault"	   },
	{ do_bad,		SIGBUS,	 "external abort on linefetch"	   },
	{ do_page_fault,	SIGSEGV, "page translation fault"	   },
	{ do_bad,		SIGBUS,	 "external abort on non-linefetch" },
	{ do_bad,		SIGSEGV, "section domain fault"		   },
	{ do_bad,		SIGBUS,	 "external abort on non-linefetch" },
	{ do_bad,		SIGSEGV, "page domain fault"		   },
	{ do_bad,		SIGBUS,	 "external abort on translation"   },
	{ do_sect_fault,	SIGSEGV, "section permission fault"	   },
	{ do_bad,		SIGBUS,	 "external abort on translation"   },
	{ do_page_fault,	SIGSEGV, "page permission fault"	   },
	/*
	 * The following are "imprecise" aborts, which are signalled by bit
	 * 10 of the FSR, and may not be recoverable.  These are only
	 * supported if the CPU abort handler supports bit 10.
	 */
	{ do_bad,		SIGBUS,  "unknown 16"			   },
	{ do_bad,		SIGBUS,  "unknown 17"			   },
	{ do_bad,		SIGBUS,  "unknown 18"			   },
	{ do_bad,		SIGBUS,  "unknown 19"			   },
	{ do_bad,		SIGBUS,  "lock abort"			   }, /* xscale */
	{ do_bad,		SIGBUS,  "unknown 21"			   },
	{ do_bad,		SIGBUS,  "imprecise external abort"	   }, /* xscale */
	{ do_bad,		SIGBUS,  "unknown 23"			   },
	{ do_bad,		SIGBUS,  "dcache parity error"		   }, /* xscale */
	{ do_bad,		SIGBUS,  "unknown 25"			   },
	{ do_bad,		SIGBUS,  "unknown 26"			   },
	{ do_bad,		SIGBUS,  "unknown 27"			   },
	{ do_bad,		SIGBUS,  "unknown 28"			   },
	{ do_bad,		SIGBUS,  "unknown 29"			   },
	{ do_bad,		SIGBUS,  "unknown 30"			   },
	{ do_bad,		SIGBUS,  "unknown 31"			   }
};

void __init
hook_fault_code(int nr, int (*fn)(unsigned long, unsigned int, struct pt_regs *),
		int sig, const char *name)
{
	if (nr >= 0 && nr < ARRAY_SIZE(fsr_info)) {
		fsr_info[nr].fn   = fn;
		fsr_info[nr].sig  = sig;
		fsr_info[nr].name = name;
	}
}

/*
 * Dispatch a data abort to the relevant handler.
 */
asmlinkage void
do_DataAbort(unsigned long addr, unsigned int fsr, struct pt_regs *regs)
{
	const struct fsr_info *inf = fsr_info + (fsr & 15) + ((fsr & (1 << 10)) >> 6);

	if (!inf->fn(addr, fsr, regs))
		return;

	printk(KERN_ALERT "Unhandled fault: %s (0x%03x) at 0x%08lx\n",
		inf->name, fsr, addr);
	force_sig(inf->sig, current);
	die_if_kernel("Oops", regs, 0);
}

asmlinkage void
do_PrefetchAbort(unsigned long addr, struct pt_regs *regs)
{
	do_translation_fault(addr, 0, regs);
}

