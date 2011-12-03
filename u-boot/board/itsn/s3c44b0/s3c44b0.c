/*
 * (C) Copyright 2005
www.itsn.cn
ITSN-Information Technology Sources Network
 */

#include <common.h>
#include <asm/hardware.h>

/*
 * Miscelaneous platform dependent initialization
 */


int board_init (void)
{
	DECLARE_GLOBAL_DATA_PTR;
	u32 temp;

	/* Configuration Port Control Register*/
	/* Port A */
	PCONA = 0x1ff;

	/* Port B */
	PCONB = 0x1Cf;
	//ATB = 0xFFFF;

	/* Port C */
	
	PCONC = 0x0f05ff55;
	PUPC=0x30f0;
	PDATC = 0x0000;
	
	

	/* Port D */
	
	PCOND = 0xaaaa;
	PUPD = 0xff;
	

	/* Port E */
	PCONE = 0x2556a;
	PDATE = 0x0;
	PUPE = 0xff;

	/* Port F */
	PCONF = 0x9256A;
	PDATF  = 0xff; /* B2-eth_reset tied high level */
	/*
	PUPF = 0x1e3;
	*/

	/* Port G */
	PUPG = 0xf;
	PCONG = 0xFFFF; /*PG0= EINT0= ETH_INT prepared for linux kernel*/

	INTMSK = 0x03fffeff;
	INTCON = 0x05;

    /*
    	Configure chip ethernet interrupt as High level
    	Port G EINT 0-7 EINT0 -> CHIP ETHERNET
    */
	temp = EXTINT;
   	temp &= ~(0x7<<4);
    temp |= (0x4<<4); /*LEVEL_HIGH*/
	EXTINT = temp;

    /*
    	Reset SMSC LAN91C96 chip
    */
    temp= PCONF;
    temp |= 0x00000040;
    PCONF = temp;

	/* Reset high */
    temp = PDATF;
    temp |= (1 << 3);
    PDATF = temp;

    /* Short delay */
    for (temp=0;temp<10;temp++)
    {
		/* NOP */
    }

    /* Reset low */
    temp = PDATF;
    temp &= ~(1 << 3);
    PDATF = temp;

	/* arch number MACH_TYPE_MBA44B0 */
	gd->bd->bi_arch_number = 178;

	/* location of boot parameters */
	gd->bd->bi_boot_params = 0x0c000100;
	
	

	return 0;
}

int dram_init (void)
{
	DECLARE_GLOBAL_DATA_PTR;

	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

	return (0);
}

void LED()
{
  PDATC = 0x0000;
  PDATC = 0x0000;
  PDATC = 0xffff;
  PDATC = 0x0000;
  PDATC = 0x0000;
}