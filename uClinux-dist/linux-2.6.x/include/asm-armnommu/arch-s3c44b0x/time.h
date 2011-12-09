/*
 *  linux/include/asm-arm/arch-s3c44b0x/time.h
 */

#ifndef __ASM_ARCH_TIME_H__
#define __ASM_ARCH_TIME_H__

#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/hardware.h>
#include <asm/arch/timex.h>

/*
 * Set up timer interrupt.
 */
#if	CONFIG_ARM_CLK_ADJUST
void s3c44b0x_systimer_setup(void);
void s3c44b0x_led_off(int);
void s3c44b0x_led_on(int);
#else
void __init s3c44b0x_systimer_setup(void);
#endif

void __inline__ s3c44b0x_systimer_start(void);

unsigned long s3c44b0x_gettimeoffset (void)
{
	return SYSREG_GETW(S3C44B0X_TCNTB5);
}

static irqreturn_t s3c44b0x_timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
#if	CONFIG_DEBUG_NICKMIT
	static int cnt = 0;
	++cnt;
	if (cnt == HZ) {
		static int stat = 0;
		cnt = 0;
		if (stat)
			s3c44b0x_led_on(0);
		else
			s3c44b0x_led_off(0);
		stat = 1 - stat;
	}
#endif
	do_timer(regs);
	do_profile(regs);
	return IRQ_HANDLED;
}

void __init time_init(void) 
{
	s3c44b0x_systimer_setup();
	/*
	 * @todo do those really need to be function pointers ?
	 */
	gettimeoffset     = s3c44b0x_gettimeoffset;
	timer_irq.handler = s3c44b0x_timer_interrupt;

	setup_irq(S3C44B0X_INTERRUPT_TIMER5, &timer_irq);
	s3c44b0x_clear_pb(S3C44B0X_INTERRUPT_TIMER5);
	s3c44b0x_unmask_irq(S3C44B0X_INTERRUPT_TIMER5);

	s3c44b0x_systimer_start();
}
#endif
