#ifndef __FOOD_H__
#define __FOOD_H__

typedef struct Food{
    int x;
    int y;
}Food;

Food *initFood();
Food *newFood(Food *old);
void clearFood(Food *food);

#endif