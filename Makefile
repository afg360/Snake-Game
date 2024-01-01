INCLUDE = include
libs = -lmingw32 -lSDL2main -lSDL2 
dyn = -L bin -lSDL2_ttf

all: obj\main.o obj\Snake.o obj\Food.o obj\utils.o obj\rendering.o
	gcc $^ -I $(INCLUDE) -L lib $(libs) $(dyn) -o .\bin\Snake_Game 

obj\menu.o : src\menu.c
	gcc $^ -c -I $(INCLUDE) -L lib $(libs) $(dyn) -o $@

obj\main.o: src\main.c
	gcc src\main.c -c -I $(INCLUDE) -o .\obj\main.o

obj\Food.o: src\Food.c
	gcc src\Food.c -c -I $(INCLUDE) -o .\obj\Food.o
	
obj\Snake.o: src\Snake.c 
	gcc src\Snake.c -c -I $(INCLUDE) -o .\obj\Snake.o

obj\rendering.o: src\rendering.c 
	gcc $^ -c -I $(INCLUDE) -L lib $(libs)  $(dyn) -o $@

obj\utils.o: src\utils.c 
	gcc $^ -c -I $(INCLUDE) -o $@

clean:
	del *.o
	del .\obj\*.o
	del .\bin\*.exe