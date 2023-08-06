#include <stdlib.h>
#include <time.h>
#include <Food.h>
#include <Constants.h>

Food *initFood(){
    Food *food = malloc(sizeof(Food));
    srand(time(0));
    int x = rand()%(WIDTH)/PIXEL_UNIT;
    srand(time(0));
    int y = rand()%(HEIGHT)/PIXEL_UNIT;
    food->x = x*PIXEL_UNIT;
    food->y = y*PIXEL_UNIT;
    return food;
}

Food *newFood(Food *old){
    Food *new;
    do{
        new = initFood();
    }
    while (old->x == new->x && old->y == new->y);
}

void clearFood(Food *food){
    free(food);
}


