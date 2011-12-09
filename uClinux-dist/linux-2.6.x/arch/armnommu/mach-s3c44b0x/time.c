/*
 * linux/arch/arm/mach-s3c44b0x/time.c
 */

#include <linux/init.h>
#include <linux/time.h>
#include <linux/timex.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>

#define S3C44B0X_SYSTIMER_DIVIDER	2
extern int s3c44b0x_fMHZ;
extern int s3c44b0x_finMHZ;

/* the system clock is in MHz unit, here I use the prescale value for 1 us resolution */

#if	CONFIG_ARM_CLK_ADJUST
void s3c44b0x_systimer_setup(void)
#else
void __init s3c44b0x_systimer_setup(void)
#endif
{
	int prescale = s3c44b0x_fMHZ / S3C44B0X_SYSTIMER_DIVIDER;
	int cnt = s3c44b0x_fMHZ * 1000000 / prescale / S3C44B0X_SYSTIMER_DIVIDER / HZ;
	
	SYSREG_CLR	(S3C44B0X_TCON,0x7<<24);			// stop timer 5			
	SYSREG_SET	(S3C44B0X_TCNTB5, cnt);
	SYSREG_OR_SET	(S3C44B0X_TCON, 2<<24);				// update timer5 counter
	
	SYSREG_OR_SET	(S3C44B0X_TCFG0, (prescale - 1) << 16);		// set prescale, bit 16-23
	SYSREG_AND_SET	(S3C44B0X_TCFG1, 0xff0fffff);			// set timer5 divider, bit 20-23.  0 for 1/2 
}

void __inline__ s3c44b0x_systimer_start(void)
{
	SYSREG_CLR	(S3C44B0X_TCON, 0x02<<24);
	SYSREG_OR_SET	(S3C44B0X_TCON, 0x05<<24);
}


