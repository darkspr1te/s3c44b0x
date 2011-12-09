/*
 *  linux/include/asm-arm/arch-s3c3410/time.h
 *
 * 2003 Thomas Eschenbacher <thomas.eschenbacher@gmx.de>
 * modifed by Hyok S. Choi <hyok.choi@samsung.com>
 *
 * Setup for 16 bit timer 0, used as system timer.
 *
 */

#ifndef __ASM_ARCH_TIME_H__
#define __ASM_ARCH_TIME_H__

#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/hardware.h>
#include <asm/arch/timex.h>

#define CLOCKS_PER_USEC	(CONFIG_ARM_CLK/1000000)

unsigned long s3c3410_gettimeoffset (void)
{
	return (inw(S3C3410X_TCNT0) / CLOCKS_PER_USEC);
}

static irqreturn_t
s3c3410_timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
    do_timer(regs);
    do_profile(regs);

    return IRQ_HANDLED;
}

#define S3C3410X_TIMER0_PRESCALER 100

/*
 * Set up timer interrupt, and return the current time in seconds.
 */

void __init  time_init (void)
{
	u_int8_t tmod;
	u_int16_t period;
	
	/*
	 * disable and clear timer 0, set to
	 * internal clock and interval mode
	 */
	tmod = S3C3410X_T16_OMS_INTRV | S3C3410X_T16_CL;
	outb(tmod, S3C3410X_TCON0);

	/* initialize the timer period and prescaler */
	period = (CONFIG_ARM_CLK/S3C3410X_TIMER0_PRESCALER)/HZ;
	outw(period, S3C3410X_TDAT0);
	outb(S3C3410X_TIMER0_PRESCALER-1, S3C3410X_TPRE0);

	/*
	 * @todo do those really need to be function pointers ?
	 */
	gettimeoffset     = s3c3410_gettimeoffset;
	timer_irq.handler = s3c3410_timer_interrupt;

	/* set up the interrupt vevtor for timer 0 match */
	setup_irq(S3C3410X_INTERRUPT_TMC0, &timer_irq);
	
	/* enable the timer IRQ */
	s3c3410_unmask_irq(S3C3410X_INTERRUPT_TMC0);

	/* let timer 0 run... */
	tmod |= S3C3410X_T16_TEN;
	tmod &= ~S3C3410X_T16_CL;
	outb(tmod, S3C3410X_TCON0);
}

#endif
