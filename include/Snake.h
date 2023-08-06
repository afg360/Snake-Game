#ifndef __SNAKE_H__
#define __SNAKE_H__
#include <stdbool.h>

typedef enum Movement{
    North, South, East, West
}Movement;

typedef struct Position{
    int x;
    int y;
}Position;

typedef struct Snake{
    Position *body;
    Position lastPos;
    int size;
    Movement movement;
}Snake;



Snake *spawn();
void move(Snake *snake);
void eat(Snake *snake);
bool checkCollision(Snake *snake);
void clearSnake(Snake *snake);

#endif