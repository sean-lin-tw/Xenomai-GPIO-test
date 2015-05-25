WORKSPACE = /home/bigshot/workspace_new

EXTRA_CFLAGS := -I${WORKSPACE}/xenomai-head/include
ARCH = arm
CROSS_COMPILE = ${WORKSPACE}/tools-master/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi-
KERNEL_DIR = ${WORKSPACE}/linux-rpi-3.8.y/build
MODULE_DIR = $(shell pwd)

obj-m += irq-gpio-rtdm.o

.PHONY: all
all:: modules

.PHONY: modules
modules:
	${MAKE} ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} -C ${KERNEL_DIR} SUBDIRS=${MODULE_DIR}  modules

.PHONY: clean
clean::
	rm -f  *.o  .*.o  .*.o.* *.ko  .*.ko  *.mod.* .*.mod.* .*.cmd *~
	rm -f Module.symvers Module.markers modules.order 
	rm -rf .tmp_versions
