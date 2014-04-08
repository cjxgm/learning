CC = gcc
LD = ld
CCFLAGS = -m32 -Wall -O0
LDFLAGS = -melf_i386 -Ttext=0

all: kernel
test: kernel
	qemu-system-i386 -kernel kernel
clean:
	rm -f *.o
cleanall: clean
	rm -f kernel
commit: cleanall
	git add -A .
	git diff --cached
	git commit -a || true

kernel.o: kernel.c makefile
	$(CC) $(CCFLAGS) -c -o $@ $<
kernel: kernel.o
	$(LD) $(LDFLAGS) -o $@ $^

