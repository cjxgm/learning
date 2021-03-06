
# configurations
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DST = mousemidi
APP = mousemidi
VER = 0.10
ARG = /dev/input/mice
FLG = -Wall -O3 -std=gnu11 -I.
LIB = -ljack -lpthread -lm

# interfaces
.PHONY: all clean cleanall rebuild package test commit install uninstall
all: config.h $(DST)
clean:
	rm -f config.h $(OBJ)
cleanall: clean
	rm -f $(DST) *.tar.gz
rebuild: cleanall all
package:
	mkdir -p $(APP)-$(VER)
	mv makefile COPYING README.md $(SRC) $(APP)-$(VER)
	tar cvfz $(APP)-$(VER).tar.gz $(APP)-$(VER)
	mv $(APP)-$(VER)/* .
	rm -rf $(APP)-$(VER)
test: all
	./$(DST) $(ARG)
commit: cleanall
	git add -A .
	git diff --cached
	git commit -a || true
install: all
	install -svm 755 ./$(DST) /usr/bin/$(DST)
uninstall: all
	rm -f /usr/bin/$(DST)

# rules
config.h: makefile
	echo "#pragma once" > config.h
	echo "#define APP_NAME \"$(APP)\"" >> config.h
	echo "#define APP_VER  \"$(VER)\"" >> config.h
$(DST): $(OBJ) makefile
	gcc -o $@ $(OBJ) $(FLG) $(LIB)
.c.o:
	gcc -c -o $@ $< $(FLG) $(LIB)
$(foreach file,$(SRC),$(eval $(shell gcc -MM $(FLG) $(file)) config.h makefile))

