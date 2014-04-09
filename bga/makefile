CC = g++
LD = ld
CCFLAGS = -m32 -Wall -O0 -std=gnu++11 -nostdinc -nostdlib -ffreestanding
LDFLAGS = -melf_i386 -Ttext=0
SRC = $(wildcard *.cc)
OBJ = $(SRC:.cc=.o)

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
.cc.o:
	$(CC) $(CCFLAGS) -c -o $@ $<
$(foreach file,$(SRC),$(eval $(shell gcc -MM $(FLG) $(file)) makefile))

