/* 
 * linux/arch/armnommu/boot/compressed/head-s5c7375.S
 * 
 * Copyright(C)2004 Hyok S. Choi, Samsung Electronics Co.,Ltd.
 * 
 * Special startup code for S5C7375.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <linux/config.h>

	.section        ".start", "ax"
/* TODO: bus and hclk adjust codes */
	/* fall back into head.S */
