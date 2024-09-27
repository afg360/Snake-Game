INCLUDE = include
OS:= $(shell uname -s)
RM=
libs =
CFLAGS=-Wall
APP = Snake_Game

ifeq ($(OS),Linux)
    # Linux-specific flags
	RM += rm -f
	libs += -lSDL2main -lSDL2 -lSDL2_ttf
endif

#NOT TESTED
ifeq ($(OS), Windows_NT)
	RM += del
	libs += -L lib -lmingw32 -lSDL2main -lSDL2 -mwindows -L bin -lSDL2_ttf
endif

all : make_dir build
	./bin/$(APP)

debug: CFLAGS += -g
debug: make_dir build_debug
	gdb ./bin/$(APP)_debug

make_dir:
	@mkdir -p ./bin
	@mkdir -p obj


build : obj/main.o obj/snake.o obj/food.o obj/utils.o obj/rendering.o obj/configure.o
	gcc $^ -I $(INCLUDE) $(libs) -o ./bin/$(APP) 

build_debug : obj/main.o obj/snake.o obj/food.o obj/utils.o obj/rendering.o obj/configure.o
	gcc $^ $(CFLAGS) -I $(INCLUDE) $(libs) -o ./bin/$(APP)_debug

obj/menu.o : src/menu.c
	gcc $^ -c $(CFLAGS) -I $(INCLUDE) $(libs) -o $@

obj/main.o: src/main.c
	gcc src/main.c -c $(CFLAGS) -I $(INCLUDE) -o ./obj/main.o

obj/food.o: src/food.c
	gcc src/food.c -c $(CFLAGS) -I $(INCLUDE) -o ./obj/food.o
	
obj/snake.o: src/snake.c 
	gcc src/snake.c -c $(CFLAGS) -I $(INCLUDE) -o ./obj/snake.o

obj/rendering.o: src/rendering.c 
	gcc $^ -c $(CFLAGS) -I $(INCLUDE) $(libs) -o $@

obj/configure.o: src/configure.c
	gcc $^ -c $(CFLAGS) -I $(INCLUDE) -o $@

obj/utils.o: src/utils.c 
	gcc $^ -c $(CFLAGS) -I $(INCLUDE) -o $@

clean:
	@rm -f *.o
	@rm -f ./obj/*.o
	@rm -f ./bin/$(APP)*

.PHONY: all build clean
