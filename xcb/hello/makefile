
# configurations
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DST = hello
APP = hello
VER = 1.0
ARG =
FLG = -Wall -O3 -std=gnu11
LIB = -lxcb


# interfaces
.PHONY: all clean cleanall rebuild package test commit
all: config.h $(DST)
clean:
	rm -f config.h $(OBJ)
cleanall: clean
	rm -f $(DST) *.tar.gz
rebuild: cleanall all
package:
	mkdir -p $(APP)-$(VER)
	mv makefile $(SRC) $(APP)-$(VER)
	tar cvfz $(APP)-$(VER).tar.gz $(APP)-$(VER)
	mv $(APP)-$(VER)/* .
	rm -rf $(APP)-$(VER)
test: all
	./$(DST) $(ARG)
commit: cleanall
	git add -A .
	git commit -a || true

# rules
config.h: makefile
	echo "#pragma once" > config.h
	echo "#define APP_NAME \"$(APP)\"" >> config.h
	echo "#define APP_VER  \"$(VER)\"" >> config.h
$(DST): $(OBJ) makefile
	gcc -o $@ $< $(FLG) $(LIB)
.c.o:
	gcc -c -o $@ $< $(FLG) $(LIB)
$(foreach file,$(SRC),$(eval $(shell gcc -MM $(FLG) $(file)) makefile))

