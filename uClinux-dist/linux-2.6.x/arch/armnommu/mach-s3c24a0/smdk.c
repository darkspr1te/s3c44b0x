/*
 *  /arch/arm/mach-s3c24a0/smdk.c
 *
 *  This file contains all SMDK24A0 specific tweaks.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  Changes
 *
 *  2004/06/10 <heechul.yun@samsung.com>   Initial CPLD IDE support for SPJ
 *  2004/06/13 <heechul.yun@samsung.com>   CPLD IDE and USB csupport for SPJ
 *  2004/06/23 <heechul.yun@samsung.com>   Added bank1 access functions and now
 *     IDE works with cs8900.
 *
 */

#include <linux/config.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/serial_core.h>

#include <asm/hardware.h>
#include <asm/setup.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/sizes.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach-types.h>

#include "generic.h"

// #define DEBUG;
#ifdef DEBUG
#  define PDEBUG(fmt, args...) \
printk(KERN_DEBUG "[%s:%d] " fmt, __PRETTY_FUNCTION__, __LINE__ , ## args)
#else
#  define PDEBUG(fmt, args...) do {} while(0)
#endif

/* global lock to protect bank1 register settings */
spinlock_t bank1_lock = SPIN_LOCK_UNLOCKED;

bank_param_t bank1_params[] =
{
        { 0x08, 0x3740 }, /* B1_IDE_PIO0 */
        { 0x08, 0x2340 }, /* B1_IDE_PIO4 */
        { 0x38, 0x3740 }, /* B1_CS89x0 */
        { 0x00, 0x2200 }, /* B1_USB2 */
};

/* bank1 state of interrupt context */
static int b1_int_state = B1_STATE_NONE;

/**
 * real_bank1_set_param: blah blah
 */
static __inline__ void real_bank1_set_param(int bw_val, int bc1_val)
{
        SROM_BW &= ~0x38;
        SROM_BW |= bw_val;
        SROM_BC1 = bc1_val;
}

/**
 * bank1_set_state - set bank1 timing
 * @state - state
 *
 * long description
 */
void bank1_set_state(int state)
{
#if 0
        unsigned long flags;

        if ( state == B1_STATE_NONE)
                return;

        if ( state > B1_STATE_LIMIT || state < B1_STATE_NONE )
                panic("bank1: Inavlid state");


        spin_lock_irqsave( &bank1_lock, flags);

        if ( !in_interrupt())
             current->bank1_state = state;

        real_bank1_set_param( bank1_params[state].bw, bank1_params[state].bc);

        spin_unlock_irqrestore( &bank1_lock, flags);
#endif

}


void bank1_set_int_state(int state)
{
#if 0
        unsigned long flags;

        if ( !in_interrupt() )
                panic("bank1_set_int_state is called fron interrupt context\n");

        spin_lock_irqsave( &bank1_lock, flags);

        b1_int_state = state; /* set */

        if ( state == B1_STATE_NONE) /* 1st level interrupt */
                state = current->bank1_state;

        if ( state != B1_STATE_NONE )
                real_bank1_set_param( bank1_params[state].bw, bank1_params[state].bc);

        spin_unlock_irqrestore( &bank1_lock, flags);
#endif
}

/*
 * bank1_get_int_state - bank1 state of interrupt context
 */
int bank1_get_int_state(void)
{
        return b1_int_state;
}

EXPORT_SYMBOL(bank1_set_state);
EXPORT_SYMBOL(bank1_set_int_state);
EXPORT_SYMBOL(bank1_get_int_state);

static int __init smdk_init(void)
{
        printk("%s: initialize smdk24a0 board\n", __func__);

        set_gpio_ctrl(SMDK_LED4 | GPIO_PULLUP_DIS | GPIO_MODE_OUT);
        set_gpio_ctrl(SMDK_LED5 | GPIO_PULLUP_DIS | GPIO_MODE_OUT);
        set_gpio_ctrl(SMDK_LED6 | GPIO_PULLUP_DIS | GPIO_MODE_OUT);
        set_gpio_ctrl(SMDK_LED7 | GPIO_PULLUP_DIS | GPIO_MODE_OUT);

        // real_bank1_set_param(0x38, 0x3740);
        real_bank1_set_param(0x38, 0x2340);
        printk("Set BANK1 register (0x%x, 0x%x)\n", SROM_BW, SROM_BC1);

        return 0;
}

__initcall(smdk_init);



MACHINE_START(S3C24A0, "Samsung-SMDK24A0")
        MAINTAINER("Heechul Yun")
        BOOT_MEM(0x10000000, 0x40000000, 0xe0000000)
        BOOT_PARAMS(0x10000100)
        INITIRQ(elfin_init_irq)
MACHINE_END

