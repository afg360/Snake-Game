INCLUDE = include
libs = -lmingw32 -lSDL2main -lSDL2

all: obj\main.o obj\Snake.o obj\Food.o obj\utils.o #perhaps we'll need more files...?
	gcc $^ -I $(INCLUDE) -L lib $(libs) -o .\bin\Snake_Game 

test:  FStester.o Snake.o Food.o #for testing purposes before the game is done
	
	gcc obj\FStester.o obj\Food.o obj\Snake.o -o bin\test -I $(INCLUDE) -L lib $(libs)

obj\main.o: src\main.c
	gcc src\main.c -c -I $(INCLUDE) -L lib $(libs) -o .\obj\main.o

obj\FStester.o: src\FStester.c
	gcc src\FStester.c -c -I $(INCLUDE) -L lib $(libs) -o .\obj\FStester.o
	
obj\Food.o: src\Food.c
	gcc src\Food.c -c -I $(INCLUDE) -o .\obj\Food.o
	
obj\Snake.o: src\Snake.c 
	gcc src\Snake.c -c -I $(INCLUDE) -o .\obj\Snake.o

obj\utils.o: src\utils.c 
	gcc $^ -c -I $(INCLUDE) -o $@

clean:
	del *.o
	del .\obj\*.o
	del .\bin\*.exe