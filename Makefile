
ifneq ($(KERNELRELEASE),) # kbuild part of makefile
obj-m += lpc31xx_clock.o
lpc31xx_clocks-objs := lpc31xx_clock.o 


else # Normal part of the Makefile

KERNELDIR = ../linux
ARCH      = $(LINTARCH)

all:
	$(MAKE) ARCH=$(ARCH) -C $(KERNELDIR) M=$(PWD)

install:
	@echo "copy modexample.ko to your module directory"

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	@rm -f Module.symvers


endif
