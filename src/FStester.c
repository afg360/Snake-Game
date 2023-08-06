#include <stdio.h>
#include "Snake.h"
#include "Food.h"

int main(){
    Snake *snake = spawn();
    

    Food *food = initFood();
    if (snake->body[0].x == food->x && snake->body[0].y == food->y){
            printf("Success!");
            return 0;
        }
    else{
        return 1;
    }
}