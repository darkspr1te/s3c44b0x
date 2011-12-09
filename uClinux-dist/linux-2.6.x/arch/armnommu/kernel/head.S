/*
 *  linux/arch/armnommu/kernel/head.S
 *
 *  Copyright (C) 1994-2002 Russell King
 *  Copyright (C) 2003,2004 Hyok S. Choi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Common kernel startup code
 *    for 32-bit CPUs which has a process ID register.
 *
 *  note:
 *  For the architectures which has NO process ID register (MMU/MPU),
 *   or which needs some architecture specific setup codes,
 *   Please create a "head.S" in each machine directories.
 */
#include <linux/config.h>
#include <linux/linkage.h>
#include <linux/init.h>

#include <asm/assembler.h>
#include <asm/mach-types.h>
#include <asm/procinfo.h>
#include <asm/ptrace.h>
#include <asm/mach/arch.h>

/*
 * Kernel startup entry point.
 * ---------------------------
 *
 * This is normally called from the decompressor code.  The requirements
 * are: MMU = off, D-cache = off, I-cache = dont care, r0 = 0,
 * r1 = machine nr.
 *
 * See linux/arch/arm/tools/mach-types for the complete list of machine
 * numbers for r1.
 *
 */
	__INIT
	.type	stext, #function
ENTRY(stext)
	mov	r12, r0
	mov	r0, #PSR_F_BIT | PSR_I_BIT | MODE_SVC	@ make sure svc mode
	msr	cpsr_c, r0			@ and all irqs disabled

	bl	__lookup_processor_type
	teq	r10, #0				@ invalid processor?
	moveq	r0, #'p'			@ yes, error 'p'
	beq	__error
	bl	__lookup_architecture_type
	teq	r7, #0				@ invalid architecture?
	moveq	r0, #'a'			@ yes, error 'a'
	beq	__error

/*
 * Set the Control Register and Read the process ID.
 */
	mrc	p15, 0, r0, c1, c0, 0		@ write control reg
#ifdef CONFIG_ALIGNMENT_TRAP
	orr	r0, r0, #2			@ ...........A.
#endif
	mcr	p15, 0, r0, c1, c0, 0		@ write control reg
	mrc	p15, 0, r3, c0, c0, 0		@ read id reg

/*
 *  r0  = processor control register
 *  r1  = machine ID
 *  r9  = processor ID
 *  r12 = value of r0 when kernel was called (currently always zero)
 */
        adr	r5, LC0
        ldmia	r5, {r5, r6, r7, r8, sp}			@ Setup stack

        /*  Clear BSS */
        mov	r4, #0
1:      cmp	r5, r7					
        strcc	r4, [r5],#4
        bcc	1b

	str	r9, [r6]			@ Save processor ID
	str	r1, [r8]			@ Save machine type

	b	start_kernel

LC0:	.long	__bss_start			@ r5
        .long	processor_id			@ r6
        .long	_end				@ r7
	.long   __machine_arch_type		@ r8
        .long	init_thread_union+8192		@ sp

/*
 * Exception handling.  Something went wrong and we can't proceed.  We
 * ought to tell the user, but since we don't have any guarantee that
 * we're even running on the right architecture, we do virtually nothing.
 *
 * r0 = ascii error character:
 *	a = invalid architecture
 *	p = invalid processor
 *	i = invalid calling convention
 *
 * Generally, only serious errors cause this.
 */
__error:
#ifdef CONFIG_DEBUG_LL
	mov	r8, r0				@ preserve r0
	adr	r0, err_str
	bl	printascii
	mov	r0, r8
	bl	printch
#endif
1:	mov	r0, r0
	b	1b

#ifdef CONFIG_DEBUG_LL
err_str:
	.asciz	"\nError: "
	.align
#endif

/*
 * Read processor ID register (CP#15, CR0), and look up in the linker-built
 * supported processor list.  Note that we can't use the absolute addresses
 * for the __proc_info lists since we aren't running with the MMU on
 * (and therefore, we are not in the correct address space).  We have to
 * calculate the offset.
 *
 * Returns:
 *	r5, r6, r7 corrupted
 *	r8  = page table flags
 *	r9  = processor ID
 *	r10 = pointer to processor structure
 */
__lookup_processor_type:
	adr	r5, 2f
	ldmia	r5, {r7, r9, r10}
	sub	r5, r5, r10			@ convert addresses
	add	r7, r7, r5			@ to our address space
	add	r10, r9, r5
	mrc	p15, 0, r9, c0, c0		@ get processor id
1:	ldmia	r10, {r5, r6}			@ value, mask
	and	r6, r6, r9			@ mask wanted bits
	teq	r5, r6
	moveq	pc, lr
	add	r10, r10, #PROC_INFO_SZ		@ sizeof(proc_info_list)
	cmp	r10, r7
	blt	1b
	mov	r10, #0				@ unknown processor
	mov	pc, lr

/*
 * Look in include/asm-armnommu/procinfo.h and arch/armnommu/kernel/arch.[ch] for
 * more information about the __proc_info and __arch_info structures.
 */
2:	.long	__proc_info_end
	.long	__proc_info_begin
	.long	2b
	.long	__arch_info_begin
	.long	__arch_info_end

/*
 * Lookup machine architecture in the linker-build list of architectures.
 * Note that we can't use the absolute addresses for the __arch_info
 * lists since we aren't running with the MMU on (and therefore, we are
 * not in the correct address space).  We have to calculate the offset.
 *
 *  r1 = machine architecture number
 * Returns:
 *  r2, r3, r4 corrupted
 *  r5 = physical start address of RAM
 *  r6 = physical address of IO
 *  r7 = byte offset into page tables for IO
 */
__lookup_architecture_type:
	adr	r4, 2b
	ldmia	r4, {r2, r3, r5, r6, r7}	@ throw away r2, r3
	sub	r5, r4, r5			@ convert addresses
	add	r4, r6, r5			@ to our address space
	add	r7, r7, r5
1:	ldr	r5, [r4]			@ get machine type
	teq	r5, r1				@ matches loader number?
	beq	2f				@ found
	add	r4, r4, #SIZEOF_MACHINE_DESC	@ next machine_desc
	cmp	r4, r7
	blt	1b
	mov	r7, #0				@ unknown architecture
	mov	pc, lr
2:	ldmib	r4, {r5, r6, r7}		@ found, get results
	mov	pc, lr
