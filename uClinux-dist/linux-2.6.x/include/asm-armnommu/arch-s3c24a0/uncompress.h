/*
 * include/asm-arm/arch-s3c24a0/uncompress.h
 *
 * $Id: uncompress.h,v 1.1 2004/06/03 01:35:47 hcyun Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */


/*
 * The following code assumes the serial port has already been
 * initialized by the bootloader. We use only UART1 on S3C24xx
 */
#include <linux/config.h>

#ifdef CONFIG_ARCH_S3C24A0

#define ULCON                   0x0
#define UTRSTAT                 0x10
#define UTXH                    0x20
#define UTRSTAT_TX_EMPTY        (1 << 2)

#define UART0                   0x44400000
#define UART1                   0x44404000

#define UART(x)                 (*(volatile unsigned long *)(serial_port + (x)))

static void puts(const char *s)
{
        unsigned long serial_port;

        do {
                serial_port = UART0;
                if (UART(ULCON) == 0x3) break;
                serial_port = UART1;
                if (UART(ULCON) == 0x3) break;
        } while (0);

        for (; *s; s++) {
                /* wait */
                while (!(UART(UTRSTAT) & UTRSTAT_TX_EMPTY));

                /* send the character out. */
                UART(UTXH) = *s;

                /* if a LF, also do CR... */
                if (*s == 10) {
                        while (!(UART(UTRSTAT) & UTRSTAT_TX_EMPTY));

                        UART(UTXH) = 13;
                }
        }
}

#else

#define UART_UTRSTAT            (*(volatile unsigned long *)0x44400010)
#define UART_UTXH               (*(volatile unsigned long *)0x44400020) /* littel endian */
#define UTRSTAT_TX_EMPTY        (1 << 2)

static void puts(const char *s)
{
        while (*s) {
                while (!(UART_UTRSTAT & UTRSTAT_TX_EMPTY));

                UART_UTXH = *s;

                if (*s == '\n') {
                        while (!(UART_UTRSTAT & UTRSTAT_TX_EMPTY));

                        UART_UTXH = '\r';
                }
                s++;
        }
        while (!(UART_UTRSTAT & UTRSTAT_TX_EMPTY));
}
#endif

/*
 * Nothing to do for these
 */
#define arch_decomp_setup()
#define arch_decomp_wdog()
