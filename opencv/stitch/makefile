
# configurations
SRC = $(wildcard *.cc)
OBJ = $(SRC:.cc=.o)
DST = opencv
APP = opencv
VER = learning
ARG =
FLG = -Wall -O3 -std=gnu++11
LIB = `pkg-config --cflags --libs opencv`


# interfaces
.PHONY: all clean cleanall rebuild package test commit
all: config.h $(DST)
clean:
	rm -f config.h $(OBJ)
	rm -f result.jpg
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
	feh result.jpg
commit: cleanall
	git add -A .
	git commit -a || true

# rules
config.h: makefile
	echo "#pragma once" > config.h
	echo "#define APP_NAME \"$(APP)\"" >> config.h
	echo "#define APP_VER  \"$(VER)\"" >> config.h
$(DST): $(OBJ) makefile
	g++ -o $@ $< $(FLG) $(LIB)
.cc.o:
	g++ -c -o $@ $< $(FLG) $(LIB)
$(foreach file,$(SRC),$(eval $(shell g++ -MM $(FLG) $(file)) makefile))

