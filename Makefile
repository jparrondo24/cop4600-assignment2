CFLAGS=-Wall -Werror -lpthread

obj-m += lkmasg1.o
lkmasg1-objs := read.o write.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc test.c -o test

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm test
