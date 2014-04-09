CC = gcc
LD = ld
CCFLAGS = -m32 -Wall -O0 -std=gnu11 -nostdinc -nostdlib -ffreestanding
LDFLAGS = -melf_i386 -Ttext=0
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

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

kernel: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^
.c.o:
	$(CC) $(CCFLAGS) -c -o $@ $<
$(foreach file,$(SRC),$(eval $(shell gcc -MM $(FLG) $(file)) makefile))

