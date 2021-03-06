/*
 * arch/ppc/kernel/head_e500.S
 *
 * Kernel execution entry point code.
 *
 *    Copyright (c) 1995-1996 Gary Thomas <gdt@linuxppc.org>
 *      Initial PowerPC version.
 *    Copyright (c) 1996 Cort Dougan <cort@cs.nmt.edu>
 *      Rewritten for PReP
 *    Copyright (c) 1996 Paul Mackerras <paulus@cs.anu.edu.au>
 *      Low-level exception handers, MMU support, and rewrite.
 *    Copyright (c) 1997 Dan Malek <dmalek@jlc.net>
 *      PowerPC 8xx modifications.
 *    Copyright (c) 1998-1999 TiVo, Inc.
 *      PowerPC 403GCX modifications.
 *    Copyright (c) 1999 Grant Erickson <grant@lcse.umn.edu>
 *      PowerPC 403GCX/405GP modifications.
 *    Copyright 2000 MontaVista Software Inc.
 *	PPC405 modifications
 *      PowerPC 403GCX/405GP modifications.
 * 	Author: MontaVista Software, Inc.
 *         	frank_rowand@mvista.com or source@mvista.com
 * 	   	debbie_chu@mvista.com
 *    Copyright 2002-2004 MontaVista Software, Inc.
 *      PowerPC 44x support, Matt Porter <mporter@kernel.crashing.org>
 *    Copyright 2004 Freescale Semiconductor, Inc
 *      PowerPC e500 modifications, Kumar Gala <kumar.gala@freescale.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/config.h>
#include <asm/processor.h>
#include <asm/page.h>
#include <asm/mmu.h>
#include <asm/pgtable.h>
#include <asm/cputable.h>
#include <asm/thread_info.h>
#include <asm/ppc_asm.h>
#include <asm/offsets.h>
#include "head_booke.h"

/* As with the other PowerPC ports, it is expected that when code
 * execution begins here, the following registers contain valid, yet
 * optional, information:
 *
 *   r3 - Board info structure pointer (DRAM, frequency, MAC address, etc.)
 *   r4 - Starting address of the init RAM disk
 *   r5 - Ending address of the init RAM disk
 *   r6 - Start of kernel command line string (e.g. "mem=128")
 *   r7 - End of kernel command line string
 *
 */
	.text
_GLOBAL(_stext)
_GLOBAL(_start)
	/*
	 * Reserve a word at a fixed location to store the address
	 * of abatron_pteptrs
	 */
	nop
/*
 * Save parameters we are passed
 */
	mr	r31,r3
	mr	r30,r4
	mr	r29,r5
	mr	r28,r6
	mr	r27,r7
	li	r24,0		/* CPU number */

/* We try to not make any assumptions about how the boot loader
 * setup or used the TLBs.  We invalidate all mappings from the
 * boot loader and load a single entry in TLB1[0] to map the
 * first 16M of kernel memory.  Any boot info passed from the
 * bootloader needs to live in this first 16M.
 *
 * Requirement on bootloader:
 *  - The page we're executing in needs to reside in TLB1 and
 *    have IPROT=1.  If not an invalidate broadcast could
 *    evict the entry we're currently executing in.
 *
 *  r3 = Index of TLB1 were executing in
 *  r4 = Current MSR[IS]
 *  r5 = Index of TLB1 temp mapping
 *
 * Later in mapin_ram we will correctly map lowmem, and resize TLB1[0]
 * if needed
 */

/* 1. Find the index of the entry we're executing in */
	bl	invstr				/* Find our address */
invstr:	mflr	r6				/* Make it accessible */
	mfmsr	r7
	rlwinm	r4,r7,27,31,31			/* extract MSR[IS] */
	mfspr	r7, SPRN_PID0
	slwi	r7,r7,16
	or	r7,r7,r4
	mtspr	SPRN_MAS6,r7
	tlbsx	0,r6				/* search MSR[IS], SPID=PID0 */
	mfspr	r7,SPRN_MAS1
	andis.	r7,r7,MAS1_VALID@h
	bne	match_TLB
	mfspr	r7,SPRN_PID1
	slwi	r7,r7,16
	or	r7,r7,r4
	mtspr	SPRN_MAS6,r7
	tlbsx	0,r6				/* search MSR[IS], SPID=PID1 */
	mfspr	r7,SPRN_MAS1
	andis.	r7,r7,MAS1_VALID@h
	bne	match_TLB
	mfspr	r7, SPRN_PID2
	slwi	r7,r7,16
	or	r7,r7,r4
	mtspr	SPRN_MAS6,r7
	tlbsx	0,r6				/* Fall through, we had to match */
match_TLB:
	mfspr	r7,SPRN_MAS0
	rlwinm	r3,r7,16,28,31			/* Extract MAS0(Entry) */

	mfspr	r7,SPRN_MAS1			/* Insure IPROT set */
	oris	r7,r7,MAS1_IPROT@h
	mtspr	SPRN_MAS1,r7
	tlbwe

/* 2. Invalidate all entries except the entry we're executing in */
	mfspr	r9,SPRN_TLB1CFG
	andi.	r9,r9,0xfff
	li	r6,0				/* Set Entry counter to 0 */
1:	lis	r7,0x1000			/* Set MAS0(TLBSEL) = 1 */
	rlwimi	r7,r6,16,12,15			/* Setup MAS0 = TLBSEL | ESEL(r6) */
	mtspr	SPRN_MAS0,r7
	tlbre
	mfspr	r7,SPRN_MAS1
	rlwinm	r7,r7,0,2,31			/* Clear MAS1 Valid and IPROT */
	cmpw	r3,r6
	beq	skpinv				/* Dont update the current execution TLB */
	mtspr	SPRN_MAS1,r7
	tlbwe
	isync
skpinv:	addi	r6,r6,1				/* Increment */
	cmpw	r6,r9				/* Are we done? */
	bne	1b				/* If not, repeat */

	/* Invalidate TLB0 */
	li      r6,0x04
	tlbivax 0,r6
#ifdef CONFIG_SMP
	tlbsync
#endif
	/* Invalidate TLB1 */
	li      r6,0x0c
	tlbivax 0,r6
#ifdef CONFIG_SMP
	tlbsync
#endif
	msync

/* 3. Setup a temp mapping and jump to it */
	andi.	r5, r3, 0x1	/* Find an entry not used and is non-zero */
	addi	r5, r5, 0x1
	lis	r7,0x1000	/* Set MAS0(TLBSEL) = 1 */
	rlwimi	r7,r3,16,12,15	/* Setup MAS0 = TLBSEL | ESEL(r3) */
	mtspr	SPRN_MAS0,r7
	tlbre

	/* Just modify the entry ID and EPN for the temp mapping */
	lis	r7,0x1000	/* Set MAS0(TLBSEL) = 1 */
	rlwimi	r7,r5,16,12,15	/* Setup MAS0 = TLBSEL | ESEL(r5) */
	mtspr	SPRN_MAS0,r7
	xori	r6,r4,1		/* Setup TMP mapping in the other Address space */
	slwi	r6,r6,12
	oris	r6,r6,(MAS1_VALID|MAS1_IPROT)@h
	ori	r6,r6,(MAS1_TSIZE(BOOKE_PAGESZ_4K))@l
	mtspr	SPRN_MAS1,r6
	mfspr	r6,SPRN_MAS2
	li	r7,0		/* temp EPN = 0 */
	rlwimi	r7,r6,0,20,31
	mtspr	SPRN_MAS2,r7
	tlbwe

	xori	r6,r4,1
	slwi	r6,r6,5		/* setup new context with other address space */
	bl	1f		/* Find our address */
1:	mflr	r9
	rlwimi	r7,r9,0,20,31
	addi	r7,r7,24
	mtspr	SRR0,r7
	mtspr	SRR1,r6
	rfi

/* 4. Clear out PIDs & Search info */
	li	r6,0
	mtspr	SPRN_PID0,r6
	mtspr	SPRN_PID1,r6
	mtspr	SPRN_PID2,r6
	mtspr	SPRN_MAS6,r6

/* 5. Invalidate mapping we started in */
	lis	r7,0x1000	/* Set MAS0(TLBSEL) = 1 */
	rlwimi	r7,r3,16,12,15	/* Setup MAS0 = TLBSEL | ESEL(r3) */
	mtspr	SPRN_MAS0,r7
	tlbre
	li	r6,0
	mtspr	SPRN_MAS1,r6
	tlbwe
	/* Invalidate TLB1 */
	li      r9,0x0c
	tlbivax 0,r9
#ifdef CONFIG_SMP
	tlbsync
#endif
	msync

/* 6. Setup KERNELBASE mapping in TLB1[0] */
	lis	r6,0x1000		/* Set MAS0(TLBSEL) = TLB1(1), ESEL = 0 */
	mtspr	SPRN_MAS0,r6
	lis	r6,(MAS1_VALID|MAS1_IPROT)@h
	ori	r6,r6,(MAS1_TSIZE(BOOKE_PAGESZ_16M))@l
	mtspr	SPRN_MAS1,r6
	li	r7,0
	lis	r6,KERNELBASE@h
	ori	r6,r6,KERNELBASE@l
	rlwimi	r6,r7,0,20,31
	mtspr	SPRN_MAS2,r6
	li	r7,(MAS3_SX|MAS3_SW|MAS3_SR)
	mtspr	SPRN_MAS3,r7
	tlbwe

/* 7. Jump to KERNELBASE mapping */
	li	r7,0
	bl	1f			/* Find our address */
1:	mflr	r9
	rlwimi	r6,r9,0,20,31
	addi	r6,r6,24
	mtspr	SRR0,r6
	mtspr	SRR1,r7
	rfi				/* start execution out of TLB1[0] entry */

/* 8. Clear out the temp mapping */
	lis	r7,0x1000	/* Set MAS0(TLBSEL) = 1 */
	rlwimi	r7,r5,16,12,15	/* Setup MAS0 = TLBSEL | ESEL(r5) */
	mtspr	SPRN_MAS0,r7
	tlbre
	mtspr	SPRN_MAS1,r8
	tlbwe
	/* Invalidate TLB1 */
	li      r9,0x0c
	tlbivax 0,r9
#ifdef CONFIG_SMP
	tlbsync
#endif
	msync

	/* Establish the interrupt vector offsets */
	SET_IVOR(0,  CriticalInput);
	SET_IVOR(1,  MachineCheck);
	SET_IVOR(2,  DataStorage);
	SET_IVOR(3,  InstructionStorage);
	SET_IVOR(4,  ExternalInput);
	SET_IVOR(5,  Alignment);
	SET_IVOR(6,  Program);
	SET_IVOR(7,  FloatingPointUnavailable);
	SET_IVOR(8,  SystemCall);
	SET_IVOR(9,  AuxillaryProcessorUnavailable);
	SET_IVOR(10, Decrementer);
	SET_IVOR(11, FixedIntervalTimer);
	SET_IVOR(12, WatchdogTimer);
	SET_IVOR(13, DataTLBError);
	SET_IVOR(14, InstructionTLBError);
	SET_IVOR(15, Debug);
	SET_IVOR(32, SPEUnavailable);
	SET_IVOR(33, SPEFloatingPointData);
	SET_IVOR(34, SPEFloatingPointRound);
	SET_IVOR(35, PerformanceMonitor);

	/* Establish the interrupt vector base */
	lis	r4,interrupt_base@h	/* IVPR only uses the high 16-bits */
	mtspr	SPRN_IVPR,r4

	/* Setup the defaults for TLB entries */
	li	r2,MAS4_TSIZED(BOOKE_PAGESZ_4K)
   	mtspr	SPRN_MAS4, r2

#if 0
	/* Enable DOZE */
	mfspr	r2,SPRN_HID0
	oris	r2,r2,HID0_DOZE@h
	mtspr	SPRN_HID0, r2
#endif

	/*
	 * This is where the main kernel code starts.
	 */

	/* ptr to current */
	lis	r2,init_task@h
	ori	r2,r2,init_task@l

	/* ptr to current thread */
	addi	r4,r2,THREAD	/* init task's THREAD */
	mtspr	SPRG3,r4

	/* stack */
	lis	r1,init_thread_union@h
	ori	r1,r1,init_thread_union@l
	li	r0,0
	stwu	r0,THREAD_SIZE-STACK_FRAME_OVERHEAD(r1)

	bl	early_init

	mfspr	r3,SPRN_TLB1CFG
	andi.	r3,r3,0xfff
	lis	r4,num_tlbcam_entries@ha
	stw	r3,num_tlbcam_entries@l(r4)
/*
 * Decide what sort of machine this is and initialize the MMU.
 */
	mr	r3,r31
	mr	r4,r30
	mr	r5,r29
	mr	r6,r28
	mr	r7,r27
	bl	machine_init
	bl	MMU_init

	/* Setup PTE pointers for the Abatron bdiGDB */
	lis	r6, swapper_pg_dir@h
	ori	r6, r6, swapper_pg_dir@l
	lis	r5, abatron_pteptrs@h
	ori	r5, r5, abatron_pteptrs@l
	lis	r4, KERNELBASE@h
	ori	r4, r4, KERNELBASE@l
	stw	r5, 0(r4)	/* Save abatron_pteptrs at a fixed location */
	stw	r6, 0(r5)

	/* Let's move on */
	lis	r4,start_kernel@h
	ori	r4,r4,start_kernel@l
	lis	r3,MSR_KERNEL@h
	ori	r3,r3,MSR_KERNEL@l
	mtspr	SRR0,r4
	mtspr	SRR1,r3
	rfi			/* change context and jump to start_kernel */

/*
 * Interrupt vector entry code
 *
 * The Book E MMUs are always on so we don't need to handle
 * interrupts in real mode as with previous PPC processors. In
 * this case we handle interrupts in the kernel virtual address
 * space.
 *
 * Interrupt vectors are dynamically placed relative to the
 * interrupt prefix as determined by the address of interrupt_base.
 * The interrupt vectors offsets are programmed using the labels
 * for each interrupt vector entry.
 *
 * Interrupt vectors must be aligned on a 16 byte boundary.
 * We align on a 32 byte cache line boundary for good measure.
 */

interrupt_base:
	/* Critical Input Interrupt */
	CRITICAL_EXCEPTION(0x0100, CriticalInput, UnknownException)

	/* Machine Check Interrupt */
	MCHECK_EXCEPTION(0x0200, MachineCheck, MachineCheckException)

	/* Data Storage Interrupt */
	START_EXCEPTION(DataStorage)
	mtspr	SPRG0, r10		/* Save some working registers */
	mtspr	SPRG1, r11
	mtspr	SPRG4W, r12
	mtspr	SPRG5W, r13
	mfcr	r11
	mtspr	SPRG7W, r11

	/*
	 * Check if it was a store fault, if not then bail
	 * because a user tried to access a kernel or
	 * read-protected page.  Otherwise, get the
	 * offending address and handle it.
	 */
	mfspr	r10, SPRN_ESR
	andis.	r10, r10, ESR_ST@h
	beq	2f

	mfspr	r10, SPRN_DEAR		/* Get faulting address */

	/* If we are faulting a kernel address, we have to use the
	 * kernel page tables.
	 */
	lis	r11, TASK_SIZE@h
	ori	r11, r11, TASK_SIZE@l
	cmplw	0, r10, r11
	bge	2f

	/* Get the PGD for the current thread */
3:
	mfspr	r11,SPRG3
	lwz	r11,PGDIR(r11)
4:
	rlwimi	r11, r10, 12, 20, 29	/* Create L1 (pgdir/pmd) address */
	lwz	r11, 0(r11)		/* Get L1 entry */
	rlwinm.	r12, r11, 0, 0, 19	/* Extract L2 (pte) base address */
	beq	2f			/* Bail if no table */

	rlwimi	r12, r10, 22, 20, 29	/* Compute PTE address */
	lwz	r11, 0(r12)		/* Get Linux PTE */

	/* Are _PAGE_USER & _PAGE_RW set & _PAGE_HWWRITE not? */
	andi.	r13, r11, _PAGE_RW|_PAGE_USER|_PAGE_HWWRITE
	cmpwi	0, r13, _PAGE_RW|_PAGE_USER
	bne	2f			/* Bail if not */

	/* Update 'changed'. */
	ori	r11, r11, _PAGE_DIRTY|_PAGE_ACCESSED|_PAGE_HWWRITE
	stw	r11, 0(r12)		/* Update Linux page table */

	/* MAS2 not updated as the entry does exist in the tlb, this
	   fault taken to detect state transition (eg: COW -> DIRTY)
	 */
	lis	r12, MAS3_RPN@h
	ori	r12, r12, _PAGE_HWEXEC | MAS3_RPN@l
	and	r11, r11, r12
	rlwimi	r11, r11, 31, 27, 27	/* SX <- _PAGE_HWEXEC */
	ori     r11, r11, (MAS3_UW|MAS3_SW|MAS3_UR|MAS3_SR)@l /* set static perms */

	/* update search PID in MAS6, AS = 0 */
	mfspr	r12, SPRN_PID0
	slwi	r12, r12, 16
	mtspr	SPRN_MAS6, r12

	/* find the TLB index that caused the fault.  It has to be here. */
	tlbsx	0, r10

	mtspr	SPRN_MAS3,r11
	tlbwe

	/* Done...restore registers and get out of here.  */
	mfspr	r11, SPRG7R
	mtcr	r11
	mfspr	r13, SPRG5R
	mfspr	r12, SPRG4R
	mfspr	r11, SPRG1
	mfspr	r10, SPRG0
	rfi			/* Force context change */

2:
	/*
	 * The bailout.  Restore registers to pre-exception conditions
	 * and call the heavyweights to help us out.
	 */
	mfspr	r11, SPRG7R
	mtcr	r11
	mfspr	r13, SPRG5R
	mfspr	r12, SPRG4R
	mfspr	r11, SPRG1
	mfspr	r10, SPRG0
	b	data_access

	/* Instruction Storage Interrupt */
	START_EXCEPTION(InstructionStorage)
	NORMAL_EXCEPTION_PROLOG
	mfspr	r5,SPRN_ESR		/* Grab the ESR and save it */
	stw	r5,_ESR(r11)
	mr      r4,r12                  /* Pass SRR0 as arg2 */
	li      r5,0                    /* Pass zero as arg3 */
	EXC_XFER_EE_LITE(0x0400, handle_page_fault)

	/* External Input Interrupt */
	EXCEPTION(0x0500, ExternalInput, do_IRQ, EXC_XFER_LITE)

	/* Alignment Interrupt */
	START_EXCEPTION(Alignment)
	NORMAL_EXCEPTION_PROLOG
	mfspr   r4,SPRN_DEAR            /* Grab the DEAR and save it */
	stw     r4,_DEAR(r11)
	addi    r3,r1,STACK_FRAME_OVERHEAD
	EXC_XFER_EE(0x0600, AlignmentException)

	/* Program Interrupt */
	START_EXCEPTION(Program)
	NORMAL_EXCEPTION_PROLOG
	mfspr	r4,SPRN_ESR		/* Grab the ESR and save it */
	stw	r4,_ESR(r11)
	addi	r3,r1,STACK_FRAME_OVERHEAD
	EXC_XFER_STD(0x0700, ProgramCheckException)

	/* Floating Point Unavailable Interrupt */
	EXCEPTION(0x0800, FloatingPointUnavailable, UnknownException, EXC_XFER_EE)

	/* System Call Interrupt */
	START_EXCEPTION(SystemCall)
	NORMAL_EXCEPTION_PROLOG
	EXC_XFER_EE_LITE(0x0c00, DoSyscall)

	/* Auxillary Processor Unavailable Interrupt */
	EXCEPTION(0x2900, AuxillaryProcessorUnavailable, UnknownException, EXC_XFER_EE)

	/* Decrementer Interrupt */
	START_EXCEPTION(Decrementer)
	NORMAL_EXCEPTION_PROLOG
	lis     r0,TSR_DIS@h            /* Setup the DEC interrupt mask */
	mtspr   SPRN_TSR,r0		/* Clear the DEC interrupt */
	addi    r3,r1,STACK_FRAME_OVERHEAD
	EXC_XFER_LITE(0x0900, timer_interrupt)

	/* Fixed Internal Timer Interrupt */
	/* TODO: Add FIT support */
	EXCEPTION(0x3100, FixedIntervalTimer, UnknownException, EXC_XFER_EE)

	/* Watchdog Timer Interrupt */
	/* TODO: Add watchdog support */
	CRITICAL_EXCEPTION(0x3200, WatchdogTimer, UnknownException)

	/* Data TLB Error Interrupt */
	START_EXCEPTION(DataTLBError)
	mtspr	SPRG0, r10		/* Save some working registers */
	mtspr	SPRG1, r11
	mtspr	SPRG4W, r12
	mtspr	SPRG5W, r13
	mfcr	r11
	mtspr	SPRG7W, r11
	mfspr	r10, SPRN_DEAR		/* Get faulting address */

	/* If we are faulting a kernel address, we have to use the
	 * kernel page tables.
	 */
	lis	r11, TASK_SIZE@h
	ori	r11, r11, TASK_SIZE@l
	cmplw	5, r10, r11
	blt	5, 3f
	lis	r11, swapper_pg_dir@h
	ori	r11, r11, swapper_pg_dir@l

	mfspr	r12,SPRN_MAS1		/* Set TID to 0 */
	li	r13,MAS1_TID@l
	andc	r12,r12,r13
	mtspr	SPRN_MAS1,r12

	b	4f

	/* Get the PGD for the current thread */
3:
	mfspr	r11,SPRG3
	lwz	r11,PGDIR(r11)

4:
	rlwimi	r11, r10, 12, 20, 29	/* Create L1 (pgdir/pmd) address */
	lwz	r11, 0(r11)		/* Get L1 entry */
	rlwinm.	r12, r11, 0, 0, 19	/* Extract L2 (pte) base address */
	beq	2f			/* Bail if no table */

	rlwimi	r12, r10, 22, 20, 29	/* Compute PTE address */
	lwz	r11, 0(r12)		/* Get Linux PTE */
	andi.	r13, r11, _PAGE_PRESENT
	beq	2f

	ori	r11, r11, _PAGE_ACCESSED
	stw	r11, 0(r12)

	 /* Jump to common tlb load */
	b	finish_tlb_load
2:
	/* The bailout.  Restore registers to pre-exception conditions
	 * and call the heavyweights to help us out.
	 */
	mfspr	r11, SPRG7R
	mtcr	r11
	mfspr	r13, SPRG5R
	mfspr	r12, SPRG4R
	mfspr	r11, SPRG1
	mfspr	r10, SPRG0
	b	data_access

	/* Instruction TLB Error Interrupt */
	/*
	 * Nearly the same as above, except we get our
	 * information from different registers and bailout
	 * to a different point.
	 */
	START_EXCEPTION(InstructionTLBError)
	mtspr	SPRG0, r10		/* Save some working registers */
	mtspr	SPRG1, r11
	mtspr	SPRG4W, r12
	mtspr	SPRG5W, r13
	mfcr	r11
	mtspr	SPRG7W, r11
	mfspr	r10, SRR0		/* Get faulting address */

	/* If we are faulting a kernel address, we have to use the
	 * kernel page tables.
	 */
	lis	r11, TASK_SIZE@h
	ori	r11, r11, TASK_SIZE@l
	cmplw	5, r10, r11
	blt	5, 3f
	lis	r11, swapper_pg_dir@h
	ori	r11, r11, swapper_pg_dir@l

	mfspr	r12,SPRN_MAS1		/* Set TID to 0 */
	li	r13,MAS1_TID@l
	andc	r12,r12,r13
	mtspr	SPRN_MAS1,r12

	b	4f

	/* Get the PGD for the current thread */
3:
	mfspr	r11,SPRG3
	lwz	r11,PGDIR(r11)

4:
	rlwimi	r11, r10, 12, 20, 29	/* Create L1 (pgdir/pmd) address */
	lwz	r11, 0(r11)		/* Get L1 entry */
	rlwinm.	r12, r11, 0, 0, 19	/* Extract L2 (pte) base address */
	beq	2f			/* Bail if no table */

	rlwimi	r12, r10, 22, 20, 29	/* Compute PTE address */
	lwz	r11, 0(r12)		/* Get Linux PTE */
	andi.	r13, r11, _PAGE_PRESENT
	beq	2f

	ori	r11, r11, _PAGE_ACCESSED
	stw	r11, 0(r12)

	/* Jump to common TLB load point */
	b	finish_tlb_load

2:
	/* The bailout.  Restore registers to pre-exception conditions
	 * and call the heavyweights to help us out.
	 */
	mfspr	r11, SPRG7R
	mtcr	r11
	mfspr	r13, SPRG5R
	mfspr	r12, SPRG4R
	mfspr	r11, SPRG1
	mfspr	r10, SPRG0
	b	InstructionStorage

#ifdef CONFIG_SPE
	/* SPE Unavailable */
	START_EXCEPTION(SPEUnavailable)
	NORMAL_EXCEPTION_PROLOG
	bne	load_up_spe
	addi    r3,r1,STACK_FRAME_OVERHEAD
	EXC_XFER_EE_LITE(0x2010, KernelSPE)
#else
	EXCEPTION(0x2020, SPEUnavailable, UnknownException, EXC_XFER_EE)
#endif /* CONFIG_SPE */

	/* SPE Floating Point Data */
#ifdef CONFIG_SPE
	EXCEPTION(0x2030, SPEFloatingPointData, SPEFloatingPointException, EXC_XFER_EE);
#else
	EXCEPTION(0x2040, SPEFloatingPointData, UnknownException, EXC_XFER_EE)
#endif /* CONFIG_SPE */

	/* SPE Floating Point Round */
	EXCEPTION(0x2050, SPEFloatingPointRound, UnknownException, EXC_XFER_EE)

	/* Performance Monitor */
	EXCEPTION(0x2060, PerformanceMonitor, UnknownException, EXC_XFER_EE)

/* Check for a single step debug exception while in an exception
 * handler before state has been saved.  This is to catch the case
 * where an instruction that we are trying to single step causes
 * an exception (eg ITLB/DTLB miss) and thus the first instruction of
 * the exception handler generates a single step debug exception.
 *
 * If we get a debug trap on the first instruction of an exception handler,
 * we reset the MSR_DE in the _exception handler's_ MSR (the debug trap is
 * a critical exception, so we are using SPRN_CSRR1 to manipulate the MSR).
 * The exception handler was handling a non-critical interrupt, so it will
 * save (and later restore) the MSR via SPRN_SRR1, which will still have
 * the MSR_DE bit set.
 */
	/* Debug Interrupt */
	START_EXCEPTION(Debug)
	CRITICAL_EXCEPTION_PROLOG

	/*
	 * If this is a single step or branch-taken exception in an
	 * exception entry sequence, it was probably meant to apply to
	 * the code where the exception occurred (since exception entry
	 * doesn't turn off DE automatically).  We simulate the effect
	 * of turning off DE on entry to an exception handler by turning
	 * off DE in the CSRR1 value and clearing the debug status.
	 */
	mfspr	r10,SPRN_DBSR		/* check single-step/branch taken */
	andis.	r10,r10,(DBSR_IC|DBSR_BT)@h
	beq+	1f
	andi.	r0,r9,MSR_PR		/* check supervisor */
	beq	2f			/* branch if we need to fix it up... */

	/* continue normal handling for a critical exception... */
1:	mfspr	r4,SPRN_DBSR
	addi	r3,r1,STACK_FRAME_OVERHEAD
	EXC_XFER_TEMPLATE(DebugException, 0x2002, \
		(MSR_KERNEL & ~(MSR_ME|MSR_DE|MSR_CE)), \
		NOCOPY, crit_transfer_to_handler, ret_from_crit_exc)

	/* here it looks like we got an inappropriate debug exception. */
2:	rlwinm	r9,r9,0,~MSR_DE		/* clear DE in the CSRR1 value */
	mtspr	SPRN_DBSR,r10		/* clear the IC/BT debug intr status */
	/* restore state and get out */
	lwz	r10,_CCR(r11)
	lwz	r0,GPR0(r11)
	lwz	r1,GPR1(r11)
	mtcrf	0x80,r10
	mtspr	CSRR0,r12
	mtspr	CSRR1,r9
	lwz	r9,GPR9(r11)

	mtspr	SPRG2,r8;		/* SPRG2 only used in criticals */
	lis	r8,crit_save@ha;
	lwz	r10,crit_r10@l(r8)
	lwz	r11,crit_r11@l(r8)
	mfspr	r8,SPRG2

	rfci
	b	.

/*
 * Local functions
 */
	/*
	 * Data TLB exceptions will bail out to this point
	 * if they can't resolve the lightweight TLB fault.
	 */
data_access:
	NORMAL_EXCEPTION_PROLOG
	mfspr	r5,SPRN_ESR		/* Grab the ESR, save it, pass arg3 */
	stw	r5,_ESR(r11)
	mfspr	r4,SPRN_DEAR		/* Grab the DEAR, save it, pass arg2 */
	andis.	r10,r5,(ESR_ILK|ESR_DLK)@h
	bne	1f
	EXC_XFER_EE_LITE(0x0300, handle_page_fault)
1:
	addi	r3,r1,STACK_FRAME_OVERHEAD
	EXC_XFER_EE_LITE(0x0300, CacheLockingException)

/*

 * Both the instruction and data TLB miss get to this
 * point to load the TLB.
 * 	r10 - EA of fault
 * 	r11 - TLB (info from Linux PTE)
 * 	r12, r13 - available to use
 * 	CR5 - results of addr < TASK_SIZE
 *	MAS0, MAS1 - loaded with proper value when we get here
 *	MAS2, MAS3 - will need additional info from Linux PTE
 *	Upon exit, we reload everything and RFI.
 */
finish_tlb_load:
	/*
	 * We set execute, because we don't have the granularity to
	 * properly set this at the page level (Linux problem).
	 * Many of these bits are software only.  Bits we don't set
	 * here we (properly should) assume have the appropriate value.
	 */

	mfspr	r12, SPRN_MAS2
	rlwimi	r12, r11, 26, 27, 31	/* extract WIMGE from pte */
	mtspr	SPRN_MAS2, r12

	bge	5, 1f

	/* addr > TASK_SIZE */
	li	r10, (MAS3_UX | MAS3_UW | MAS3_UR)
	andi.	r13, r11, (_PAGE_USER | _PAGE_HWWRITE | _PAGE_HWEXEC)
	andi.	r12, r11, _PAGE_USER	/* Test for _PAGE_USER */
	iseleq	r12, 0, r10
	and	r10, r12, r13
	srwi	r12, r10, 1
	or	r12, r12, r10	/* Copy user perms into supervisor */
	b	2f

	/* addr <= TASK_SIZE */
1:	rlwinm	r12, r11, 31, 29, 29	/* Extract _PAGE_HWWRITE into SW */
	ori	r12, r12, (MAS3_SX | MAS3_SR)

2:	rlwimi	r11, r12, 0, 20, 31	/* Extract RPN from PTE and merge with perms */
	mtspr	SPRN_MAS3, r11
	tlbwe

	/* Done...restore registers and get out of here.  */
	mfspr	r11, SPRG7R
	mtcr	r11
	mfspr	r13, SPRG5R
	mfspr	r12, SPRG4R
	mfspr	r11, SPRG1
	mfspr	r10, SPRG0
	rfi					/* Force context change */

#ifdef CONFIG_SPE
/* Note that the SPE support is closely modeled after the AltiVec
 * support.  Changes to one are likely to be applicable to the
 * other!  */
load_up_spe:
/*
 * Disable SPE for the task which had SPE previously,
 * and save its SPE registers in its thread_struct.
 * Enables SPE for use in the kernel on return.
 * On SMP we know the SPE units are free, since we give it up every
 * switch.  -- Kumar
 */
	mfmsr	r5
	oris	r5,r5,MSR_SPE@h
	mtmsr	r5			/* enable use of SPE now */
	isync
/*
 * For SMP, we don't do lazy SPE switching because it just gets too
 * horrendously complex, especially when a task switches from one CPU
 * to another.  Instead we call giveup_spe in switch_to.
 */
#ifndef CONFIG_SMP
	lis	r3,last_task_used_spe@ha
	lwz	r4,last_task_used_spe@l(r3)
	cmpi	0,r4,0
	beq	1f
	addi	r4,r4,THREAD	/* want THREAD of last_task_used_spe */
	SAVE_32EVR(0,r10,r4)
   	evxor	evr10, evr10, evr10	/* clear out evr10 */
	evmwumiaa evr10, evr10, evr10	/* evr10 <- ACC = 0 * 0 + ACC */
	li	r5,THREAD_ACC
   	evstddx	evr10, r4, r5		/* save off accumulator */
	lwz	r5,PT_REGS(r4)
	lwz	r4,_MSR-STACK_FRAME_OVERHEAD(r5)
	lis	r10,MSR_SPE@h
	andc	r4,r4,r10	/* disable SPE for previous task */
	stw	r4,_MSR-STACK_FRAME_OVERHEAD(r5)
1:
#endif /* CONFIG_SMP */
	/* enable use of SPE after return */
	oris	r9,r9,MSR_SPE@h
	mfspr	r5,SPRG3		/* current task's THREAD (phys) */
	li	r4,1
	li	r10,THREAD_ACC
	stw	r4,THREAD_USED_SPE(r5)
	evlddx	evr4,r10,r5
	evmra	evr4,evr4
	REST_32EVR(0,r10,r5)
#ifndef CONFIG_SMP
	subi	r4,r5,THREAD
	stw	r4,last_task_used_spe@l(r3)
#endif /* CONFIG_SMP */
	/* restore registers and return */
2:	REST_4GPRS(3, r11)
	lwz	r10,_CCR(r11)
	REST_GPR(1, r11)
	mtcr	r10
	lwz	r10,_LINK(r11)
	mtlr	r10
	REST_GPR(10, r11)
	mtspr	SRR1,r9
	mtspr	SRR0,r12
	REST_GPR(9, r11)
	REST_GPR(12, r11)
	lwz	r11,GPR11(r11)
	SYNC
	rfi



/*
 * SPE unavailable trap from kernel - print a message, but let
 * the task use SPE in the kernel until it returns to user mode.
 */
KernelSPE:
	lwz	r3,_MSR(r1)
	oris	r3,r3,MSR_SPE@h
	stw	r3,_MSR(r1)	/* enable use of SPE after return */
	lis	r3,87f@h
	ori	r3,r3,87f@l
	mr	r4,r2		/* current */
	lwz	r5,_NIP(r1)
	bl	printk
	b	ret_from_except
87:	.string	"SPE used in kernel  (task=%p, pc=%x)  \n"
	.align	4,0

#endif /* CONFIG_SPE */

/*
 * Global functions
 */

/*
 * extern void loadcam_entry(unsigned int index)
 *
 * Load TLBCAM[index] entry in to the L2 CAM MMU
 */
_GLOBAL(loadcam_entry)
	lis	r4,TLBCAM@ha
	addi	r4,r4,TLBCAM@l
	mulli	r5,r3,20
	add	r3,r5,r4
	lwz	r4,0(r3)
	mtspr	SPRN_MAS0,r4
	lwz	r4,4(r3)
	mtspr	SPRN_MAS1,r4
	lwz	r4,8(r3)
	mtspr	SPRN_MAS2,r4
	lwz	r4,12(r3)
	mtspr	SPRN_MAS3,r4
	tlbwe
	isync
	blr

/*
 * extern void giveup_altivec(struct task_struct *prev)
 *
 * The e500 core does not have an AltiVec unit.
 */
_GLOBAL(giveup_altivec)
	blr

#ifdef CONFIG_SPE
/*
 * extern void giveup_spe(struct task_struct *prev)
 *
 */
_GLOBAL(giveup_spe)
	mfmsr	r5
	oris	r5,r5,MSR_SPE@h
	SYNC
	mtmsr	r5			/* enable use of SPE now */
	isync
	cmpi	0,r3,0
	beqlr-				/* if no previous owner, done */
	addi	r3,r3,THREAD		/* want THREAD of task */
	lwz	r5,PT_REGS(r3)
	cmpi	0,r5,0
	SAVE_32EVR(0, r4, r3)
   	evxor	evr6, evr6, evr6	/* clear out evr6 */
	evmwumiaa evr6, evr6, evr6	/* evr6 <- ACC = 0 * 0 + ACC */
	li	r4,THREAD_ACC
   	evstddx	evr6, r4, r3		/* save off accumulator */
	mfspr	r6,SPRN_SPEFSCR
	stw	r6,THREAD_SPEFSCR(r3)	/* save spefscr register value */
	beq	1f
	lwz	r4,_MSR-STACK_FRAME_OVERHEAD(r5)
	lis	r3,MSR_SPE@h
	andc	r4,r4,r3		/* disable SPE for previous task */
	stw	r4,_MSR-STACK_FRAME_OVERHEAD(r5)
1:
#ifndef CONFIG_SMP
	li	r5,0
	lis	r4,last_task_used_spe@ha
	stw	r5,last_task_used_spe@l(r4)
#endif /* CONFIG_SMP */
	blr
#endif /* CONFIG_SPE */

/*
 * extern void giveup_fpu(struct task_struct *prev)
 *
 * The e500 core does not have an FPU.
 */
_GLOBAL(giveup_fpu)
	blr

/*
 * extern void abort(void)
 *
 * At present, this routine just applies a system reset.
 */
_GLOBAL(abort)
	li	r13,0
        mtspr   SPRN_DBCR0,r13		/* disable all debug events */
	mfmsr	r13
	ori	r13,r13,MSR_DE@l	/* Enable Debug Events */
	mtmsr	r13
        mfspr   r13,SPRN_DBCR0
        lis	r13,(DBCR0_IDM|DBCR0_RST_CHIP)@h
        mtspr   SPRN_DBCR0,r13

_GLOBAL(set_context)

#ifdef CONFIG_BDI_SWITCH
	/* Context switch the PTE pointer for the Abatron BDI2000.
	 * The PGDIR is the second parameter.
	 */
	lis	r5, abatron_pteptrs@h
	ori	r5, r5, abatron_pteptrs@l
	stw	r4, 0x4(r5)
#endif
	mtspr	SPRN_PID,r3
	isync			/* Force context change */
	blr

/*
 * We put a few things here that have to be page-aligned. This stuff
 * goes at the beginning of the data segment, which is page-aligned.
 */
	.data
_GLOBAL(sdata)
_GLOBAL(empty_zero_page)
	.space	4096
_GLOBAL(swapper_pg_dir)
	.space	4096

	.section .bss
/* Stack for handling critical exceptions from kernel mode */
critical_stack_bottom:
	.space 4096
critical_stack_top:
	.previous

/* Stack for handling machine check exceptions from kernel mode */
mcheck_stack_bottom:
	.space 4096
mcheck_stack_top:
	.previous

/*
 * This area is used for temporarily saving registers during the
 * critical and machine check exception prologs. It must always
 * follow the page aligned allocations, so it starts on a page
 * boundary, ensuring that all crit_save areas are in a single
 * page.
 */

/* crit_save */
_GLOBAL(crit_save)
	.space  4
_GLOBAL(crit_r10)
	.space	4
_GLOBAL(crit_r11)
	.space	4
_GLOBAL(crit_sprg0)
	.space	4
_GLOBAL(crit_sprg1)
	.space	4
_GLOBAL(crit_sprg4)
	.space	4
_GLOBAL(crit_sprg5)
	.space	4
_GLOBAL(crit_sprg7)
	.space	4
_GLOBAL(crit_pid)
	.space	4
_GLOBAL(crit_srr0)
	.space	4
_GLOBAL(crit_srr1)
	.space	4

/* mcheck_save */
_GLOBAL(mcheck_save)
	.space  4
_GLOBAL(mcheck_r10)
	.space	4
_GLOBAL(mcheck_r11)
	.space	4
_GLOBAL(mcheck_sprg0)
	.space	4
_GLOBAL(mcheck_sprg1)
	.space	4
_GLOBAL(mcheck_sprg4)
	.space	4
_GLOBAL(mcheck_sprg5)
	.space	4
_GLOBAL(mcheck_sprg7)
	.space	4
_GLOBAL(mcheck_pid)
	.space	4
_GLOBAL(mcheck_srr0)
	.space	4
_GLOBAL(mcheck_srr1)
	.space	4
_GLOBAL(mcheck_csrr0)
	.space	4
_GLOBAL(mcheck_csrr1)
	.space	4

/*
 * This space gets a copy of optional info passed to us by the bootstrap
 * which is used to pass parameters into the kernel like root=/dev/sda1, etc.
 */
_GLOBAL(cmd_line)
	.space	512

/*
 * Room for two PTE pointers, usually the kernel and current user pointers
 * to their respective root page table.
 */
abatron_pteptrs:
	.space	8


