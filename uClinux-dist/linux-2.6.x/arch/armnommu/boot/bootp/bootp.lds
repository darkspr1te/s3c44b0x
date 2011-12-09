/*
 *  linux/arch/armnommu/boot/bootp/bootp.lds
 *
 *  Copyright (C) 2000-2002 Russell King
 *  Modified by Hyok S. Choi, 2004
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
ENTRY(_start)
SECTIONS
{
  . = 0;
  _text = .;
  .text : {
   _stext = .;
   *(.start)
   arch/armnommu/boot/bootp/kernel.o
   . = ALIGN(32);
   initrd_start = .;
   arch/armnommu/boot/bootp/initrd.o
   initrd_len = . - initrd_start;
   . = ALIGN(32);
   _etext = .;
  }
  
  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }
}
