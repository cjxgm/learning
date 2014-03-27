# configurations
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DST = gst.so
APP = gst
VER = 0.10
ARG =
FLG = -march=native -Wall -O3 -std=gnu11 -fPIC -shared
LIB = `pkg-config --libs --cflags gstreamer-1.0` -llua

# interfaces
.PHONY: all clean cleanall rebuild package test commit
all: $(DST)
clean:
	rm -f $(OBJ)
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
	git diff --cached
	git commit -a || true

# rules
$(DST): $(OBJ) makefile
	gcc -o $@ $< $(FLG) $(LIB)
.c.o:
	gcc -c -o $@ $< $(FLG) $(LIB)
$(foreach file,$(SRC),$(eval $(shell gcc -MM $(FLG) $(file)) makefile))

