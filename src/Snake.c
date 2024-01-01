#include <stdlib.h>
//#include "Linked_List.h"//import doubly linked list instead
#include <Snake.h>
#include <Constants.h>

Snake *spawn(){
    Position position = {0,0};//WIDTH/2+1, HEIGHT/2+1};
    Snake *snake = malloc(sizeof(Snake));
    snake->body = malloc(sizeof(Position));
    snake->body[0] = position;
    //snake->head = position;
    //snake->tail = position;
    //snake->lastPos = NULL; there is no lastPos
    snake->size = 1;
    snake->movement = South;
    return snake;
}

void move(Snake *snake){
    snake->lastPos = snake->body[snake->size-1];
    Movement movement = snake->movement;
    Position prev = snake->body[0];
    switch (movement){
        case North:
            for (int i = 0; i<snake->size; i++){
                if (i != 0){
                    Position curr = snake->body[i];
                    snake->body[i] = prev;
                    prev = curr;
                }
                else snake->body[i].y-= PIXEL_UNIT;
            }
            break;
        case South:
            for (int i = 0; i<snake->size; i++){
                if (i != 0){
                    Position curr = snake->body[i];
                    snake->body[i] = prev;
                    prev = curr;
                }
                else snake->body[i].y+= PIXEL_UNIT;
            }
            break;
        case East:
            for (int i = 0; i<snake->size; i++){
                if (i != 0){
                    Position curr = snake->body[i];
                    snake->body[i] = prev;
                    prev = curr;
                }
                else{
                    snake->body[i].x+= PIXEL_UNIT;
                }
            }
            break;
        case West:
            for (int i = 0; i<snake->size; i++){
                if (i != 0){
                    Position curr = snake->body[i];
                    snake->body[i] = prev;
                    prev = curr;
                }
                else{
                     snake->body[i].x-=PIXEL_UNIT;
                    }
            }
            break;
    }

}
void eat(Snake *snake){
    snake->size++;
    snake->body = realloc(snake->body, sizeof(Position)*snake->size);
    snake->body[snake->size-1] = snake->lastPos;
}

bool checkCollision(Snake *snake){
    if (snake->body[0].x < 0 || snake->body[0].x >= WIDTH || snake->body[0].y < 0 || snake->body[0].y >= HEIGHT){
            return true;
        }
    for (int i = 1; i<snake->size; i++){
        if (snake->body[0].x == snake->body[i].x && snake->body[0].y == snake->body[i].y){
            return true;
        }
    }
    return false;
}

void clearSnake(Snake *snake){
    free(snake->body);
    free(snake);
}