CC=g++

MAKEFLAGS := --jobs=$(shell nproc)

CFLAGS= -shared -fPIC -g

LDFLAGS= -l:libGLEW.so.2.1 -lSDL2 funchook/libfunchook.a funchook/libdistorm.a -g 

#OBJ_FILES=$(shell find . -name '*.cpp' | sed -e "s/.\///" | sed -e "s/$$/.o/") # Build all .cpp files
OBJ_FILES=main.cpp.o # Unity build
OBJS=$(addprefix obj/, $(OBJ_FILES)) 
BIN=tf2.so

.PHONY: all clean debug

#-------------------------------------------------------------------------------

all: $(BIN)

clean:
	rm -f $(OBJS)
	rm -f $(BIN)

#-------------------------------------------------------------------------------

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

obj/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<
