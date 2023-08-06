all: main.o Snake.o Food.o #perhaps we'll need more files...?
	gcc obj\main.o obj\Food.o obj\Snake.o -o .\bin\Snake -I include -L lib -lmingw32 -lSDL2main -lSDL2

test:  FStester.o Snake.o Food.o #for testing purposes before the game is done
	
	gcc obj\FStester.o obj\Food.o obj\Snake.o -o bin\test -I include -L lib -lmingw32 -lSDL2main -lSDL2


main.o: src\main.c
	gcc src\main.c -c -I include -L lib -lmingw32 -lSDL2main -lSDL2 -o .\obj\main.o
	
	

FStester.o: src\FStester.c
	gcc src\FStester.c -c -I include -L lib -lmingw32 -lSDL2main -lSDL2 -o .\obj\FStester.o
	
Food.o: src\Food.c
	gcc src\Food.c -c -I include -o .\obj\Food.o
	
Snake.o: src\Snake.c 
	gcc src\Snake.c -c -I include -o .\obj\Snake.o

clean:
	rm *.o

cleanobj:
	rm .\obj\*.o