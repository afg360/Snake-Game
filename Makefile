INCLUDE = include
libs =-lmingw32 -lSDL2main -lSDL2 
dyn =-L bin -lSDL2_ttf

all: obj\main.o obj\snake.o obj\food.o obj\utils.o obj\rendering.o obj\configure.o
	gcc $^ -I $(INCLUDE) -L lib $(libs) $(dyn) -o .\bin\snake_Game 

obj\menu.o : src\menu.c
	gcc $^ -c -I $(INCLUDE) -L lib $(libs) $(dyn) -o $@

obj\main.o: src\main.c
	gcc src\main.c -c -I $(INCLUDE) -o .\obj\main.o

obj\food.o: src\food.c
	gcc src\food.c -c -I $(INCLUDE) -o .\obj\food.o
	
obj\snake.o: src\snake.c 
	gcc src\snake.c -c -I $(INCLUDE) -o .\obj\snake.o

obj\rendering.o: src\rendering.c 
	gcc $^ -c -I $(INCLUDE) -L lib $(libs)  $(dyn) -o $@

obj\configure.o: src\configure.c
	gcc $^ -c -I $(INCLUDE) -o $@

obj\utils.o: src\utils.c 
	gcc $^ -c -I $(INCLUDE) -o $@

clean:
	del *.o
	del .\obj\*.o
	del .\bin\*.exe
