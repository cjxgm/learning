
# configurations
CC = g++
LD = g++
SUFFIX = cc
SRC = $(wildcard *.$(SUFFIX))
OBJ = $(SRC:.$(SUFFIX)=.o)
DST = variant
APP = $(DST)
VER = 0.10
ARG =
FLG = -Wall -g -std=gnu++0x -I.
LIB =

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
$(DST): $(OBJ)
	$(LD) -o $@ $^ $(FLG) $(LIB)
.$(SUFFIX).o:
	$(CC) -c -o $@ $< $(FLG) $(LIB)
$(foreach file,$(SRC),$(eval $(shell $(CC) -MM $(FLG) $(file)) config.h makefile))

