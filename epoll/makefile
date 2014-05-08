
# configurations
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DST = epoll
APP = $(DST)
VER = 0.10
ARG =
FLG = -Wall -O3 -march=native -std=gnu11 -pthread
LIB =

# interfaces
.PHONY: all clean cleanall rebuild package test commit install uninstall
all: $(DST)
clean:
	rm -f $(OBJ)
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
$(DST): $(OBJ) makefile
	gcc -o $@ $(OBJ) $(FLG) $(LIB)
.c.o:
	gcc -c -o $@ $< $(FLG) $(LIB)
$(foreach file,$(SRC),$(eval $(shell gcc -MM $(FLG) $(file)) makefile))

