
obj-m := megaraid_sas_spindown.o
megaraid_sas_spindown-objs := megaraid_sas_base.o megaraid_sas_fusion.o \
	megaraid_sas_fp.o megaraid_sas_debugfs.o

# normal makefile
KernelDIR := /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KernelDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KernelDIR) M=$(PWD) clean
