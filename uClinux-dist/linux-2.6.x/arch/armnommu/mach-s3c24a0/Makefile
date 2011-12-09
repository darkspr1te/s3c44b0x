$(Error:FIXME)

#
# Makefile for the linux kernel.
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).

USE_STANDARD_AS_RULE := true


obj-y :=
obj-m := registers.o
obj-n :=
obj-  :=

# Common support (must be linked before board specific support)
obj-y += irq.o clocks.o 

# Specific board support
obj-$(CONFIG_ARCH_SMDK24A0) += smdk.o

# LEDs support
leds-y := leds.o
leds-$(CONFIG_ARCH_SMDK24A0) += leds-smdk.o
obj-$(CONFIG_LEDS) += $(leds-y)
 
