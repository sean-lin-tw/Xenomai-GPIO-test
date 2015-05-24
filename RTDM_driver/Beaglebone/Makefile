EXTRA_CFLAGS := -I/home/emp-chen/git/bbb/xenomai-2.6.4/include
 
ifneq (${KERNELRELEASE},)
	obj-m += oscillateur-gpio-rtdm.o
 
else
	ARCH ?= arm
	CROSS_COMPILE ?= /home/emp-chen/linux-arm-genuhbi-
	KERNEL_DIR = /home/emp-chen/git/bbb/beagle-kernel/kernel
	MODULE_DIR := $(shell pwd)
	CFLAGS := -Wall -g 
 
.PHONY: all
all:: modules
 
.PHONY: modules
modules:
	${MAKE} ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} -C ${KERNEL_DIR} SUBDIRS=${MODULE_DIR}  modules
 
XENOCONFIG=/home/emp-chen/git/bbb/xenomai-2.6.4/RPI/usr/xenomai/bin/xeno-config
 
.PHONY: clean
clean::
  rm -f  *.o  .*.o  .*.o.* *.ko  .*.ko  *.mod.* .*.mod.* .*.cmd *~
  rm -f Module.symvers Module.markers modules.order 
  rm -rf .tmp_versions
endif



