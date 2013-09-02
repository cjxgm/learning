
# configurations
SRC = $(wildcard *.cc)
OBJ = $(SRC:.cc=.o)
DST = output
APP = output
VER = 1.0
ARG =
FLG = -Wall -O3 -std=gnu++0x
LIB = `pkg-config --cflags --libs gtkmm-3.0`


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
$(DST): $(OBJ)
	g++ -o $@ $^ $(FLG) $(LIB)
.cc.o:
	g++ -c -o $@ $< $(FLG) $(LIB)
$(foreach file,$(SRC),$(eval $(shell g++ -MM $(FLG) $(file))))

