/*
 * (C) Copyright 2006
 * ITSN
 *
 * http://www.itsn.cn
 * http://www.itsn.cn/bbs
 * mailto:admin@itsn.cn
 *
 * Configuation settings for the s3c44b0x board.
 */


#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * If we are developing, we might want to start armboot from ram
 * so we MUST NOT initialize critical regs like mem-timing ...*/
 
#define CONFIG_INIT_CRITICAL	1        /* undef for developing */

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_ARM7			1	/* This is a ARM7 CPU	*/
#define CONFIG_ITSNs3c44b0	1	/* on an hfrks3c44b0 Board      */
#define CONFIG_ARM_THUMB	1	/* this is an ARM7TDMI */
#undef  CONFIG_ARM7_REVD	 	/* disable ARM720 REV.D Workarounds */

#define CONFIG_S3C44B0_CLOCK_SPEED	60 /* we have a 60Mhz(40Mhz) S3C44B0*/


#undef CONFIG_USE_IRQ			/* don't need them anymore */


/*
 * Size of malloc() pool
 */
#define CFG_MONITOR_LEN		(256 * 1024)	/* Reserve 256 kB for Monitor	*/
#define CFG_ENV_SIZE		(64*1024)		/* 1024 bytes may be used for env vars*/
//#define CFG_ENV_SIZE		1024		/* 1024 bytes may be used for env vars*/

#define CFG_MALLOC_LEN		(CFG_ENV_SIZE + 128*1024 )
#define CFG_GBL_DATA_SIZE	128	/* size in bytes reserved for initial data */

#define CFG_ENV_IS_IN_FLASH	  1  /* use FLASH for environment vars */
#define CFG_ENV_ADDR	(PHYS_FLASH_1+0x40000)  
#define CFG_ENV_OFFSET	0x40000 
#define CONFIG_AUTO_COMPLETE

/*
 * Hardware drivers
 */
#define CONFIG_DRIVER_RTL8019
#define RTL8019_BASE		0x06000300 /* base address         */
#define RTL8019_BUS32		0
#define CONFIG_SMC_USE_16_BIT
#undef  CONFIG_SHOW_ACTIVITY
#define CONFIG_NET_RETRY_COUNT		10	   /* # of retries          */

/*
 * select serial console configuration
 */
#define CONFIG_SERIAL1		1	/* we use Serial line 1 */

/* allow to overwrite serial and ethaddr */
#define CFG_ENV_OVERWRITE

#define CONFIG_BAUDRATE		115200

#define CONFIG_BOOTP_MASK       (CONFIG_BOOTP_DEFAULT|CONFIG_BOOTP_BOOTFILESIZE)

#define CONFIG_COMMANDS		( CONFIG_CMD_DFL | \
								CFG_CMD_DATE | \
								CFG_CMD_ELF	| \
								CFG_CMD_NET    | \
								CFG_CMD_EEPROM	| \
								CFG_CMD_I2C		| \
								CFG_CMD_FAT | \
								CFG_CMD_JFFS2)
								
								

								
/* this must be included AFTER the definition of CONFIG_COMMANDS (if any) */
#include <cmd_confdefs.h>

#define CONFIG_BOOTDELAY	3
#define CONFIG_ETHADDR	00:50:c2:1e:af:fb
#define CONFIG_BOOTARGS  "devfs=mount root=ramfs console=ttyS0,9600"

#define CONFIG_ETHADDR	00:50:c2:1e:af:fb
#define CONFIG_NETMASK  255.255.255.0
#define CONFIG_IPADDR   192.168.0.30
#define CONFIG_SERVERIP	192.168.0.10
#define CONFIG_BOOTFILE	"u-boot.bin"
#define CONFIG_BOOTCOMMAND	"bootm 0x50000"    
/*
 * Miscellaneous configurable options
 */
#define	CFG_LONGHELP				/* undef to save memory	*/
#define	CFG_PROMPT		"itsn_44b0=>"	/* Monitor Command Prompt	*/
#define	CFG_CBSIZE		256		/* Console I/O Buffer Size	*/
#define	CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16) /* Print Buffer Size */
#define	CFG_MAXARGS		 100		/* max number of command args	*/
#define CFG_BARGSIZE		CFG_CBSIZE	/* Boot Argument Buffer Size	*/

#define CFG_MEMTEST_START	0x0C400000	/* memtest works on	*/
#define CFG_MEMTEST_END		0x0C800000	/* 4 ... 8 MB in DRAM	*/

#undef  CFG_CLKS_IN_HZ		/* everything, incl board info, in Hz */

#define	CFG_LOAD_ADDR		0x0c008000	/* default load address	*/

#define	CFG_HZ				1000		/* 1 kHz */

						/* valid baudrates */
#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }
#define CFG_LED_FLASH  1

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128*1024)	/* regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(4*1024)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	(4*1024)	/* FIQ stack */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1	   /* we have 1 banks of DRAM */
#define PHYS_SDRAM_1		0x0c000000 /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE	0x00800000 /* 8 MB */

#define PHYS_FLASH_1		0x00000000 /* Flash Bank #1 */
#define PHYS_FLASH_SIZE		0x00200000 /* 2 MB */

#define CFG_FLASH_BASE		PHYS_FLASH_1

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
/*-----------------------------------------------------------------------
 * FLASH organization
 */
#define CFG_MAX_FLASH_BANKS	1	/* max number of memory banks		*/
#define CFG_MAX_FLASH_SECT	256	/* max number of sectors on one chip	*/

#define CFG_FLASH_ERASE_TOUT	4120000	/* Timeout for Flash Erase (in ms)	*/
#define CFG_FLASH_WRITE_TOUT	4000	/* Timeout for Flash Write (in ms)	*/

#define CFG_FLASH_WORD_SIZE	unsigned short	/* flash word size (width)	*/
#define CFG_FLASH_ADDR0		0x5555	/* 1st address for flash config cycles	*/
#define CFG_FLASH_ADDR1		0x2AAA	/* 2nd address for flash config cycles	*/
/*
 * The following defines are added for buggy IOP480 byte interface.
 * All other boards should use the standard values (CPCI405 etc.)
 */
#define CFG_FLASH_READ0		0x0000	   /* 0 is standard			*/
#define CFG_FLASH_READ1		0x0001	   /* 1 is standard			*/
#define CFG_FLASH_READ2		0x0002	   /* 2 is standard			*/
#define CONFIG_ITSN
#define CFG_FLASH_EMPTY_INFO		/* print 'E' for empty sector on flinfo */

/*-----------------------------------------------------------------------
 * Environment Variable setup
 */

//#define CFG_ENV_IS_IN_EEPROM	1	/* use EEPROM for environment vars */
//#define CFG_ENV_OFFSET		0x0	/* environment starts at the beginning of the EEPROM */

/*-----------------------------------------------------------------------
 * I2C EEPROM (STM24C02W6) for environment
 */
#define CONFIG_HARD_I2C			/* I2c with hardware support */
#define CFG_I2C_SPEED		400000	/* I2C speed and slave address */
#define CFG_I2C_SLAVE		0xFE

#define CFG_I2C_EEPROM_ADDR	0xA8	/* EEPROM STM24C02W6		*/
#define CFG_I2C_EEPROM_ADDR_LEN 1	/* Bytes of address		*/
/* mask of address bits that overflow into the "EEPROM chip address"	*/
#define CFG_I2C_EEPROM_ADDR_OVERFLOW	0x07
#define CFG_EEPROM_PAGE_WRITE_BITS 4	/* The Catalyst CAT24WC08 has	*/
					/* 16 byte page write mode using*/
					/* last 4 bits of the address	*/
#define CFG_EEPROM_PAGE_WRITE_DELAY_MS	10   /* and takes up to 10 msec */
#define CFG_EEPROM_PAGE_WRITE_ENABLE

/* Flash banks JFFS2 should use */


#define CFG_JFFS2_FIRST_BANK    0
#define CFG_JFFS2_FIRST_SECTOR	2
#define CFG_JFFS2_NUM_BANKS     1


/*
	Linux TAGs (see lib_arm/armlinux.c)
*/
#define CONFIG_CMDLINE_TAG
#undef  CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

#endif	/* __CONFIG_H */
