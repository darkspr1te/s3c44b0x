/*
 *  linux/drivers/serial/t32.c
 *
 *  serial port emulation driver for the TRACE32 Terminal.
 *
 * JTAG1 protocol version for ICD:
 * 	Copyright (C) 2003, 2004 Hyok S. Choi (hyok.choi@samsung.com)
 * 	SAMSUNG ELECTRONICS Co.,Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   Oct-2003 Hyok S. Choi	Created
 *   Feb-2004 Hyok S. Choi 	Updated for serial_core.c and 2.6 kernel
 *   Apr-2004 Hyok S. Choi 	xmit_string_CR added
 *
 */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/tty.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/serial.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/major.h>

#include <asm/io.h>
#include <asm/irq.h>

#include <linux/serial_core.h>

#if 0
	/* real irq interrupt used */
	#define T32_IRQ_USED
#else
	/* scheduled work used */
	#undef T32_IRQ_USED
#endif

#ifndef  T32_IRQ_USED
static struct work_struct t32_poll_task;
static void t32_poll(void *data);
#endif

#define UART_NR			1	/* we have only one JTAG port */

#define SERIAL_T32_NAME	"ttyJ"
#define SERIAL_T32_MAJOR	4
#define SERIAL_T32_MINOR	64

static int __inline__ __check_JTAG_RX_FLAG(void)
{
	int __ret=0;
	__asm__ __volatile__(
		"	mrc		p14, 0, %0, c0, c0 	@ read comms control reg\n"
		"	and		%0, %0, #1		@ jtag read buffer status"
		: "=r" (__ret)
		);

	/* if   __ret 	== 0 : no input yet
				== 1 : a character pending */
	return __ret;	
}

static void __inline__ __get_JTAG_RX(volatile char *p)
{
	__asm__ __volatile__(
		"	mrc		p14, 0, r3, c1, c0 	@ read comms data reg to r5\n"
		"	strb 		r3, 	[%0]		@ str a char"
		: /* no output */
		: "r" (p)
		: "r3",  "memory");
}


static int __inline__ __check_JTAG_TX_FLAG(void)
{
	int __ret=0;
	__asm__ __volatile__(
		"	mrc		p14, 0, %0, c0, c0 	@ read comms control reg\n"
		"	and 		%0, %0, #2		@ the read buffer status"
		: "=r" (__ret)
		);

	/* if   __ret 	== 0 : tx is available
				== 2 : tx busy */
	return __ret;	
}

void xmit_string(char *p, int len)
{
#ifndef CONFIG_JTAG_T32_OUTPUT_DISABLE
	/*
		r0 = string	; string address
		r1 = 2		; state check bit (write)
		r4 = *string	; character
		r7 = 0		; count
	*/
		__asm__ __volatile__(
			"	mov r7, #0\n"
			"1: 	mrc	p14, 0, r3, c0, c0 	@ read comms control reg\n"
			"	and r3, r3, #2			@ the write buffer status\n"
			"	cmp r3, #2			@ is it available?\n"
			"	beq 1b				@ is not, wait till then\n"
			"	ldrb r4, [%0]			@ load a char\n"
			"	mcr p14, 0, r4, c1, c0	@ write it\n"
			"	add %0, %0, #1		@ str address increase one\n"
			"	add r7, r7, #1			@ count increase\n"
			"	cmp r7, %1			@ compare with str length\n"
			"	bne 1b				@ if it is not yet, loop"
			: /* no output register */
			: "r" (p), "r" (len)
			: "r7", "r3", "r4");
#endif
}

void xmit_string_CR(char *p, int len)
{
#ifndef CONFIG_JTAG_T32_OUTPUT_DISABLE
	/*
		r0 = string	; string address
		r1 = 2		; state check bit (write)
		r4 = *string	; character
		r7 = 0		; count
	*/
		__asm__ __volatile__(
			"	mov r7, #0\n"
			"	ldrb r4, [%0]			@ load a char\n"
			"1: 	mrc	p14, 0, r3, c0, c0 	@ read comms control reg\n"
			"	and r3, r3, #2			@ the write buffer status\n"
			"	cmp r3, #2			@ is it available?\n"
			"	beq 1b				@ is not, wait till then\n"
			"	mcr p14, 0, r4, c1, c0	@ write it\n"
			"	cmp r4, #0x0a		@ is it LF?\n"
			"	bne 2f				@ if it is not, continue\n"
			"	mov r4, #0x0d		@ set the CR\n"
			"	b   1b				@ loop for writing CR\n"			
			"2:	ldrb r4, [%0, #1]!		@ load a char\n"
			"	add r7, r7, #1			@ count increase\n"
			"	cmp r7, %1			@ compare with str length\n"
			"	bne 1b				@ if it is not yet, loop"
			: /* no output register */
			: "r" (p), "r" (len)
			: "r7", "r3", "r4");
#endif
}


static void
t32_stop_tx(struct uart_port *port, unsigned int tty_stop)
{
}

static inline void
t32_transmit_buffer(struct uart_port *port)
{
	struct circ_buf *xmit = &port->info->xmit;

	int pendings = uart_circ_chars_pending(xmit);

	if(pendings + xmit->tail > UART_XMIT_SIZE)
	{
		xmit_string(&(xmit->buf[xmit->tail]), UART_XMIT_SIZE - xmit->tail);
		xmit_string(&(xmit->buf[0]), xmit->head);
	} else
		xmit_string(&(xmit->buf[xmit->tail]), pendings);
	
	xmit->tail = (xmit->tail + pendings) & (UART_XMIT_SIZE-1);
        port->icount.tx += pendings;

	if (uart_circ_empty(xmit))
		t32_stop_tx(port, 0); 
}

static inline void
t32_transmit_x_char(struct uart_port *port)
{
	xmit_string(&port->x_char, 1);
	port->icount.tx++;
	port->x_char = 0;
}

static void
t32_start_tx(struct uart_port *port, unsigned int tty_start)
{
    t32_transmit_buffer(port);
}

static void
t32_stop_rx(struct uart_port *port)
{
}

static void
t32_enable_ms(struct uart_port *port)
{
}

static inline void
t32_rx_chars(struct uart_port *port)
{
	unsigned char ch;
	struct tty_struct *tty = port->info->tty;

	/*
	 * check input.
	 * checking JTAG flag is better to resolve the status test.
	 * incount is NOT used for JTAG1 protocol.
	 */

	if (__check_JTAG_RX_FLAG())
		/* if   __ret 	== 0 : no input yet
					== 1 : a character pending */
	{
		/* for JTAG 1 protocol, incount is always 1. */
		__get_JTAG_RX(&ch);
		if (tty->flip.count < TTY_FLIPBUF_SIZE) {
			*tty->flip.char_buf_ptr++ = ch;
			*tty->flip.flag_buf_ptr++ = TTY_NORMAL;
			port->icount.rx++;
			tty->flip.count++;
		} 
		tty_flip_buffer_push(tty);
	}
}

static inline void
t32_overrun_chars(struct uart_port *port)
{
	port->icount.overrun++;
}

static inline void
t32_tx_chars(struct uart_port *port)
{
	struct circ_buf *xmit = &port->info->xmit;

	if (port->x_char) {
		t32_transmit_x_char(port);
		return; 
	}

	if (uart_circ_empty(xmit) || uart_tx_stopped(port)) {
		t32_stop_tx(port, 0);
		return;
	}

	t32_transmit_buffer(port);

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);
}

#ifdef T32_IRQ_USED /* real IRQ used */
static irqreturn_t
t32_int(int irq, void *dev_id, struct pt_regs *regs)
{
	struct uart_port *port = dev_id;
	int handled = 0;

	spin_lock(&port->lock);
	
	t32_rx_chars(port);
	t32_tx_chars(port);

	handled = 1;
	spin_unlock(&port->lock);
	
	return IRQ_RETVAL(handled);
}

#else /* emulation by scheduled work */
static void
t32_poll(void *data)
{
	struct uart_port *port = data;

	spin_lock(&port->lock);
	
	t32_rx_chars(port);
	t32_tx_chars(port);

	schedule_delayed_work(&t32_poll_task, 1);

	spin_unlock(&port->lock);
	
}
#endif /* end of T32_IRQ_USED */

static unsigned int
t32_tx_empty(struct uart_port *port)
{
	return TIOCSER_TEMT;
}

static unsigned int
t32_get_mctrl(struct uart_port *port)
{
	return 0;
}

static void
t32_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
}

static void
t32_break_ctl(struct uart_port *port, int break_state)
{
}

static int t32_startup(struct uart_port *port)
{
#ifdef T32_IRQ_USED /* real IRQ used */
	int retval;

	/* Allocate the IRQ */
	retval = request_irq(port->irq, t32_int, SA_INTERRUPT,
			     "serial_t32", port);
	if (retval)
		return retval;
#else /* emulation */
	/* Initialize the work, and shcedule it. */
	INIT_WORK(&t32_poll_task, t32_poll, port);
	schedule_delayed_work(&t32_poll_task, 1);
#endif

	return 0;
}

static void t32_shutdown(struct uart_port *port)
{
}

static void
t32_set_termios(struct uart_port *port, struct termios *termios,
		   struct termios *old)
{
#ifdef T32_IRQ_USED
	unsigned long flags;
#endif
	unsigned int baud, quot;

	/*
	 * We don't support parity, stop bits, or anything other
	 * than 8 bits, so clear these termios flags.
	 */
	termios->c_cflag &= ~(CSIZE | CSTOPB | PARENB | PARODD | CREAD);
	termios->c_cflag |= CS8;

	/*
	 * We don't appear to support any error conditions either.
	 */
	termios->c_iflag &= ~(INPCK | IGNPAR | IGNBRK | BRKINT);

	/*
	 * Ask the core to calculate the divisor for us.
	 */
	baud = uart_get_baud_rate(port, termios, old, 0, port->uartclk/16); 
	quot = uart_get_divisor(port, baud);

#ifdef T32_IRQ_USED
	spin_lock_irqsave(&port->lock, flags);
#endif

	uart_update_timeout(port, termios->c_cflag, baud);

#ifdef T32_IRQ_USED
	spin_unlock_irqrestore(&port->lock, flags);
#endif
}

static const char *t32_type(struct uart_port *port)
{
	return port->type == PORT_T32_JTAG1 ? "T32" : NULL;
}

static void t32_release_port(struct uart_port *port)
{
}

static int t32_request_port(struct uart_port *port)
{
	return 0;
}

/*
 * Configure/autoconfigure the port.
 */
static void t32_config_port(struct uart_port *port, int flags)
{
        if (flags & UART_CONFIG_TYPE) {
                port->type = PORT_T32_JTAG1;
                t32_request_port(port);
        }
}
        
/*      
 * verify the new serial_struct (for TIOCSSERIAL).
 */     
static int t32_verify_port(struct uart_port *port, struct serial_struct *ser)
{
        int ret = 0;
        if (ser->type != PORT_UNKNOWN && ser->type != PORT_T32_JTAG1)
                ret = -EINVAL;
        if (ser->irq < 0 || ser->irq >= NR_IRQS)
                ret = -EINVAL;
        if (ser->baud_base < 9600)
                ret = -EINVAL;
	return ret;
}

static struct uart_ops t32_pops = {
	.tx_empty	= t32_tx_empty,
	.set_mctrl	= t32_set_mctrl,
	.get_mctrl	= t32_get_mctrl,
	.stop_tx	= t32_stop_tx,
	.start_tx	= t32_start_tx,
	.stop_rx	= t32_stop_rx,
	.enable_ms	= t32_enable_ms,
	.break_ctl	= t32_break_ctl,
	.startup	= t32_startup,
	.shutdown	= t32_shutdown,
	.set_termios	= t32_set_termios,
	.type		= t32_type,
	.release_port  = t32_release_port,
	.request_port = t32_request_port,
	.config_port = t32_config_port,
	.verify_port = t32_verify_port,
};

static struct uart_port t32_ports[UART_NR] = {
	{
		.membase	= (char*)0x12345678,	/* we need these garbages */
		.mapbase	= 0x12345678,		/* for serial_core.c */
		.iotype		= SERIAL_IO_MEM,	
#ifdef T32_IRQ_USED
		.irq			= INT_N_EXT0,
#else
		.irq			= 0,
#endif
		.uartclk		= 14745600,			 
		.fifosize		= 0,
		.ops			= &t32_pops,
		.flags		= ASYNC_BOOT_AUTOCONF,
		.line			= 0,
	},
};


#ifdef CONFIG_SERIAL_T32_CONSOLE

static void
t32_console_write(struct console *co, const char *s, unsigned int count)
{
	xmit_string_CR((char*)s, count);
}

/*
 * Read the current UART setup.
 */
static void __init
t32_console_get_options(struct uart_port *port, int *baud, int *parity, int *bits)
{
	*baud = 9600;
	*parity = 'n';
	*bits = 8;
}

static int __init
t32_console_setup(struct console *co, char *options)
{
	struct uart_port *port;
	int baud = 9600;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	if (co->index >= UART_NR)
		co->index = 0;
	port = &t32_ports[co->index];

	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);
	else
		t32_console_get_options(port, &baud, &parity, &bits);

	return uart_set_options(port, co, baud, parity, bits, flow);
}

extern struct uart_driver t32_reg;
static struct console t32_console = {
	.name		= SERIAL_T32_NAME,
	.write		= t32_console_write,
	.device		= uart_console_device,
	.setup		= t32_console_setup,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
	.data		= &t32_reg,
};

static int __init t32_console_init(void)
{
	register_console(&t32_console);
	return 0;
}
console_initcall(t32_console_init);

#define T32_CONSOLE		&t32_console
#else
#define T32_CONSOLE		NULL
#endif

static struct uart_driver t32_reg = {
	.owner			= THIS_MODULE,
	.driver_name		= SERIAL_T32_NAME,
	.dev_name		= SERIAL_T32_NAME,
	.major			= SERIAL_T32_MAJOR,
	.minor			= SERIAL_T32_MINOR,
	.nr				= UART_NR,
	.cons			= T32_CONSOLE,
};

static int __init
t32_init(void)
{
	int ret;

	printk(KERN_INFO "T32: JTAG1 Serial emulation driver driver $Revision: 1.1 $\n");

	ret = uart_register_driver(&t32_reg);
	if (ret == 0) {
		int i;

		for (i = 0; i < UART_NR; i++)
			uart_add_one_port(&t32_reg, &t32_ports[i]);
	}
	return ret;
}

__initcall(t32_init);

MODULE_DESCRIPTION("T32 JTAG1 serial and console emulation driver");
MODULE_AUTHOR("Hyok S. Choi <hyok.choi@samsung.com>");
MODULE_SUPPORTED_DEVICE("ttyJ");
MODULE_LICENSE("GPL");
