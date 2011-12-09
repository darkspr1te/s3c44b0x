/*
 * linux/include/asm-armnommu/arch-espd_4510b/time.h
 *
 * Copyright (c) 2004	Cucy Systems (http://www.cucy.com)
 * Curt Brune <curt@cucy.com>
 *
 * 2003 Thomas Eschenbacher <thomas.eschenbacher@gmx.de>
 * modifed by Hyok S. Choi <hyok.choi@samsung.com>
 *
 * Setup for 32 bit timer 0, used as system timer.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __ASM_ARCH_TIME_H__
#define __ASM_ARCH_TIME_H__

#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/hardware.h>
#include <asm/arch/timex.h>
#include <asm/mach/irq.h>

#define CLOCKS_PER_USEC	(CONFIG_ARM_CLK/1000000)

static volatile unsigned long timer_cnt;

unsigned long s3c4510b_gettimeoffset (void)
{
	unsigned long usec;

	/* returns microseconds -- timer 1 is free running in countdown mode */
	usec = 0xFFFFFFFF - inl( REG_TCNT1);
	usec /= CLOCKS_PER_USEC;
	
	return usec;
}

static irqreturn_t
s3c4510b_timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{

	timer_cnt++;

#ifdef CONFIG_ARCH_ESPD_4510B
	if ( ! (timer_cnt % (HZ/4))) {
		LED_TOGGLE(0);
	}
#endif
	do_timer(regs);
	do_profile(regs);

	return IRQ_HANDLED;
}

/*
 * Set up timer interrupt
 */

void __init  time_init (void)
{
	u_int32_t period;
	
	/*
	 * disable and clear timers 0 and 1.  set both timers to
	 * interval mode.
	 */
	outl( 0x0, REG_TMOD);
	/* clear any pending interrupts */
	outl( 0x1FFFFF, REG_INTPEND);

	timer_cnt = 0;

	/* initialize the timer period */
	period = (CLOCK_TICK_RATE / HZ);
	outl( period, REG_TDATA0);

	/* set timer1 to continually count down from FFFFFFFF */
	outl( 0xFFFFFFFF, REG_TDATA1);

//	printk(KERN_INFO "time_init():  TICK_RATE: %u, HZ: %u, period: %u\n", CLOCK_TICK_RATE, HZ, period);

	gettimeoffset     = s3c4510b_gettimeoffset;
	timer_irq.handler = s3c4510b_timer_interrupt;

	/* set up the interrupt vevtor for timer 0 match */
	setup_irq( INT_TIMER0, &timer_irq);
	
	/* enable the timer IRQ */
	INT_ENABLE( INT_TIMER0);

	/* let timer 0 run... */
	outl( TM0_RUN | TM1_RUN, REG_TMOD);
}

#endif
