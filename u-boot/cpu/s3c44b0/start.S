/*
 * Startup Code for S3C44B0 CPU-core
 *
 * (C) Copyright 2004
 * www.21spacetime.net
 *

 */


#include <config.h>
#include <version.h>


/*
 * Jump vector table
 */


.globl _start
_start:	b       reset
	ldr pc,=HandleUndef
	ldr pc,=HandleSWI
	ldr pc,=HandlePabort
	ldr pc,=HandleDabort
	b .
	ldr pc,=HandleIRQ
	ldr pc,=HandleFIQ
	ldr pc,=HandleEINT0 /*mGA H/W interrupt vector table*/
	ldr pc,=HandleEINT1
	ldr pc,=HandleEINT2
	ldr pc,=HandleEINT3
	ldr pc,=HandleEINT4567
	ldr pc,=HandleTICK /*mGA*/
	b .
	b .
	ldr pc,=HandleZDMA0 /*mGB*/
	ldr pc,=HandleZDMA1
	ldr pc,=HandleBDMA0
	ldr pc,=HandleBDMA1
	ldr pc,=HandleWDT
	ldr pc,=HandleUERR01 /*mGB*/
	b .
	b .
	ldr pc,=HandleTIMER0 /*mGC*/
	ldr pc,=HandleTIMER1
	ldr pc,=HandleTIMER2
	ldr pc,=HandleTIMER3
	ldr pc,=HandleTIMER4
	ldr pc,=HandleTIMER5 /*mGC*/
	b .
	b .
	ldr pc,=HandleURXD0 /*mGD*/
	ldr pc,=HandleURXD1
	ldr pc,=HandleIIC
	ldr pc,=HandleSIO
	ldr pc,=HandleUTXD0
	ldr pc,=HandleUTXD1 /*mGD*/
	b .
	b .
	ldr pc,=HandleRTC /*mGKA*/
	b .
	b .
	b . 
	b .
	b . /*mGKA*/
	b .
	b .
	ldr pc,=HandleADC /*mGKB*/
	b .
	b .
	b .
	b .
	b . /*mGKB*/
	b .
	b .
	.equ HandleReset, 0xc000000
	.equ HandleUndef,0xc000004
	.equ HandleSWI, 0xc000008
	.equ HandlePabort, 0xc00000c
	.equ HandleDabort, 0xc000010
	.equ HandleReserved, 0xc000014
	.equ HandleIRQ, 0xc000018
	.equ HandleFIQ, 0xc00001c
	.equ HandleADC, 0xc000020
	.equ HandleRTC, 0xc000024
	.equ HandleUTXD1, 0xc000028
	.equ HandleUTXD0, 0xc00002c
	.equ HandleSIO, 0xc000030
	.equ HandleIIC, 0xc000034
	.equ HandleURXD1, 0xc000038
	.equ HandleURXD0, 0xc00003c
	.equ HandleTIMER5, 0xc000040
	.equ HandleTIMER4, 0xc000044
	.equ HandleTIMER3, 0xc000048
	.equ HandleTIMER2, 0xc00004c
	.equ HandleTIMER1, 0xc000050
	.equ HandleTIMER0, 0xc000054
	.equ HandleUERR01, 0xc000058
	.equ HandleWDT, 0xc00005c
	.equ HandleBDMA1, 0xc000060
	.equ HandleBDMA0, 0xc000064
	.equ HandleZDMA1, 0xc000068
	.equ HandleZDMA0, 0xc00006c
	.equ HandleTICK, 0xc000070
	.equ HandleEINT4567, 0xc000074
	.equ HandleEINT3, 0xc000078
	.equ HandleEINT2, 0xc00007c
	.equ HandleEINT1, 0xc000080
	.equ HandleEINT0, 0xc000084

	.balignl 16,0xdeadbeef


/*
 *************************************************************************
 *
 * Startup Code (reset vector)
 *
 * do important init only if we don't start from memory!
 * relocate u-boot to ram
 * setup stack
 * jump to second stage
 *
 *************************************************************************
 */

_TEXT_BASE:
	.word	TEXT_BASE

.globl _armboot_start
_armboot_start:
	.word _start

/*
 * These are defined in the board-specific linker script.
 */
.globl _bss_start
_bss_start:
	.word __bss_start

.globl _bss_end
_bss_end:
	.word _end

#ifdef CONFIG_USE_IRQ
/* IRQ stack memory (calculated at run-time) */
.globl IRQ_STACK_START
IRQ_STACK_START:
	.word	0x0badc0de

/* IRQ stack memory (calculated at run-time) */
.globl FIQ_STACK_START
FIQ_STACK_START:
	.word 0x0badc0de
#endif


/*
 * the actual reset code
 */

reset:
	/*
	 * set the cpu to SVC32 mode
	 */
	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0,#0x13
	msr	cpsr,r0

	/*
	 * we do sys-critical inits only at reboot,
	 * not when booting from ram!
	 */



#ifdef CONFIG_INIT_CRITICAL
	bl	cpu_init_crit
	/*
	 * before relocating, we have to setup RAM timing
	 * because memory timing is board-dependend, you will
	 * find a memsetup.S in your board directory.
	 */
	bl	memsetup
#endif

relocate:				/* relocate U-Boot to RAM	    */
	adr	r0, _start		/* r0 <- current position of code   */
	ldr	r1, _TEXT_BASE		/* test if we run from flash or RAM */
	cmp     r0, r1                  /* don't reloc during debug         */
	beq     stack_setup

	ldr	r2, _armboot_start
	ldr	r3, _bss_start
	sub	r2, r3, r2		/* r2 <- size of armboot            */
	add	r2, r0, r2		/* r2 <- source end address         */

copy_loop:
	ldmia	r0!, {r3-r10}		/* copy from source address [r0]    */
	stmia	r1!, {r3-r10}		/* copy to   target address [r1]    */
	cmp	r0, r2			/* until source end addreee [r2]    */
	ble	copy_loop

/*
	now copy to sram the interrupt vector
*/
	adr	r0, real_vectors
	add	r2, r0, #1024
	ldr	r1, =0x0c000000
	add	r1, r1, #0x08
vector_copy_loop:
	ldmia	r0!, {r3-r10}
	stmia	r1!, {r3-r10}
	cmp	r0, r2
	ble	vector_copy_loop

	/* Set up the stack						    */
stack_setup:
	ldr	r0, _TEXT_BASE		/* upper 128 KiB: relocated uboot   */
	sub	r0, r0, #CFG_MALLOC_LEN	/* malloc area                      */
	sub	r0, r0, #CFG_GBL_DATA_SIZE /* bdinfo                        */
#ifdef CONFIG_USE_IRQ
	sub	r0, r0, #(CONFIG_STACKSIZE_IRQ+CONFIG_STACKSIZE_FIQ)
#endif
	sub	sp, r0, #12		/* leave 3 words for abort-stack    */

	ldr	pc, _start_armboot

_start_armboot:	.word start_armboot


/*
 *************************************************************************
 *
 * CPU_init_critical registers
 *
 * setup important registers
 * setup memory timing
 *
 *************************************************************************
 */

#define INTCON (0x01c00000+0x200000)
#define INTMSK (0x01c00000+0x20000c)
#define LOCKTIME (0x01c00000+0x18000c)
#define PLLCON (0x01c00000+0x180000)
#define CLKCON (0x01c00000+0x180004)
#define WTCON (0x01c00000+0x130000)
#define BDIDES0 0x1f80008
#define BDIDES1 0x1f80028



cpu_init_crit:
	/* disable watch dog */
	ldr 	r0, =WTCON
	ldr	r1, =0x0
	str	r1, [r0]

	/*
	 * mask all IRQs by clearing all bits in the INTMRs
	 */
	ldr	r1,=INTMSK
	ldr	r0, =0x03fffeff		
	str	r0, [r1]


	/* Set Clock Control Register */
	ldr	r1, =LOCKTIME
	ldrb	r0, =800
	strb	r0, [r1]

	ldr	r1, =PLLCON

#if CONFIG_S3C44B0_CLOCK_SPEED==60
   
	ldr	r0, =0x88042 /* 60MHz (Quartz=10MHz) */
#elif CONFIG_S3C44B0_CLOCK_SPEED==75
	ldr	r0, =0xac042  /* 75MHz  */
#else
# error CONFIG_S3C44B0_CLOCK_SPEED undefined
#endif

	str	r0, [r1]

	ldr	r1,=CLKCON
	ldr	r0, =0x7ff8
	str	r0, [r1]
	
	ldr	r0, =BDIDES0       
	ldr	r1, =0x40000000  //BDIDESn reset value should be 0x40000000	 
	str	r1, [r0]

	ldr	r0, =BDIDES1      
	ldr	r1, =0x40000000   //BDIDESn reset value should be 0x40000000	 
	str	r1, [r0]
	
	mov	pc, lr


/*************************************************/
/*	interrupt vectors	*/
/*************************************************/
real_vectors:
	b	reset
	b	undefined_instruction
	b	software_interrupt
	b	prefetch_abort
	b	data_abort
	b	not_used
	b	irq
	b	fiq

/*************************************************/

undefined_instruction:
	mov	r6, #3
	b	reset

software_interrupt:
	mov	r6, #4
	b	reset

prefetch_abort:
	mov	r6, #5
	b	reset

data_abort:
	mov	r6, #6
	b	reset

not_used:
	/* we *should* never reach this */
	mov	r6, #7
	b	reset

irq:
	mov	r6, #8
	b	reset

fiq:
	mov	r6, #9
	b	reset
