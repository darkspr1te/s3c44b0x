/*
 *  linux/arch/armnommu/mm/fault-common.c
 *
 *  Copyright (C) 1995  Linus Torvalds
 *  Modifications for ARM processor (c) 1995-2001 Russell King
 *  Modifications for nommu or non-paged, Hyok S. Choi, 2003
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/config.h>
#include <linux/module.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/ptrace.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/init.h>

#include <asm/system.h>
#include <asm/pgtable.h>
#include <asm/tlbflush.h>
#include <asm/uaccess.h>

#include "fault.h"

/*
 * "code" is actually the FSR register.  Bit 11 set means the
 * instruction was performing a write.
 */
#define DO_COW(code)		((code) & (1 << 11))
#define READ_FAULT(code)	(!DO_COW(code))

/*
 * Oops.  The kernel tried to access some page that wasn't present.
 */
static void
__do_kernel_fault(struct mm_struct *mm, unsigned long addr, unsigned int fsr,
		  struct pt_regs *regs)
{
	/*
	 * Are we prepared to handle this kernel fault?
	 */
	if (fixup_exception(regs))
		return;

	/*
	 * No handler, we'll have to terminate things with extreme prejudice.
	 */
	bust_spinlocks(1);
	printk(KERN_ALERT
		"Unable to handle kernel %s at virtual address %08lx\n",
		(addr < PAGE_SIZE) ? "NULL pointer dereference" :
		"paging request", addr);

	die("Oops", regs, fsr);
	bust_spinlocks(0);
	do_exit(SIGKILL);
}

/*
 * Something tried to access memory that isn't in our memory map..
 * User mode accesses just cause a SIGSEGV
 */
static void
__do_user_fault(struct task_struct *tsk, unsigned long addr,
		unsigned int fsr, int code, struct pt_regs *regs)
{
	struct siginfo si;

#ifdef CONFIG_DEBUG_USER
	printk(KERN_DEBUG "%s: unhandled page fault at 0x%08lx, code 0x%03x\n",
	       tsk->comm, addr, fsr);
	show_regs(regs);
#endif

	tsk->thread.address = addr;
	tsk->thread.error_code = fsr;
	tsk->thread.trap_no = 14;
	si.si_signo = SIGSEGV;
	si.si_errno = 0;
	si.si_code = code;
	si.si_addr = (void *)addr;
	force_sig_info(SIGSEGV, &si, tsk);
}

void
do_bad_area(struct task_struct *tsk, struct mm_struct *mm, unsigned long addr,
	    unsigned int fsr, struct pt_regs *regs)
{
	/*
	 * If we are in kernel mode at this point, we
	 * have no context to handle this fault with.
	 */
	if (user_mode(regs))
		__do_user_fault(tsk, addr, fsr, SEGV_MAPERR, regs);
	else
		__do_kernel_fault(mm, addr, fsr, regs);
}

int do_page_fault(unsigned long addr, unsigned int fsr, struct pt_regs *regs)
{
	printk("do_page_fault? : %s %d\n",__FILE__,__LINE__);
	return 0;
}

int do_translation_fault(unsigned long addr, unsigned int fsr,
			 struct pt_regs *regs)
{
	printk("do_translation_fault? : %s %d\n",__FILE__,__LINE__);
	return 0;
}
