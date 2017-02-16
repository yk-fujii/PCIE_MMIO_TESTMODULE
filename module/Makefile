TARGET := testio
$(TARGET)-y := testio.o
obj-m := testiorw.o

SYSSRC = /lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS=

all:
	+make -C $(SYSSRC) M=$(PWD) modules

clean:
	+make -C $(SYSSRC) M=$(PWD) clean
	rm -f *~

