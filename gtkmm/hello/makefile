
# configurations
SRC = $(wildcard *.cc)
OBJ = ${SRC:.cc=.o}
DST = output
APP = output
VER = 1.0
ARG =
FLAGS = -Wall -O3 -std=gnu++0x
FLAGS += `pkg-config --cflags --libs gtkmm-3.0`

# interfaces
all: config.h $(DST)
config.h: makefile
	echo "#pragma once" > config.h
	echo "#define APP_NAME \"$(APP)\"" >> config.h
	echo "#define APP_VER  \"$(VER)\"" >> config.h
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

# rules
$(DST): $(OBJ)
	g++ -o $@ $^ $(FLAGS)
.cc.o:
	g++ -c -o $@ $< $(FLAGS)

