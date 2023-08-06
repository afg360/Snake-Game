#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <Snake.h>
#include <Food.h>
#include <Constants.h>
 //pixel size of a square

void ErrMessage(char message[]);
void FPSLimit (unsigned int limit);

int score = 0;

int main(int argv, char *arcg[]){
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_Log("Error: Initialisation of SDL -> %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    
    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) != 0){ //perhaps make a fullscreen window
        ErrMessage("Error: Window Creation and rendering");
    }
    /*
    if (SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE) != 0){//make yellow color for lines
        ErrMessage("Error: Changing color for the render");
    }
    //Horizontal lines
    for (int i = 0; i<HEIGHT/PIXEL_UNIT; i++){
        if (SDL_RenderDrawLine(renderer, 0, PIXEL_UNIT*i, WIDTH, PIXEL_UNIT*i) != 0){
        ErrMessage("Error: Impossible to draw a line");
        }
    }
    //Vertical lines
    for (int j = 0; j<WIDTH/PIXEL_UNIT; j++){
        if (SDL_RenderDrawLine(renderer, PIXEL_UNIT*j, 0, PIXEL_UNIT*j, HEIGHT) != 0){
        ErrMessage("Error: Impossible to draw a line");
        }
    }
    SDL_RenderPresent(renderer); //updates rendering
    SDL_RenderClear(renderer);
    */
    //refresh/update time frame to setup too so that snake doesnt move too fast
    Snake *snake = spawn();
    Food *food = initFood();
    move(snake);
    SDL_bool running = SDL_TRUE;
    while (running){
        SDL_Event event;
        printf("Coords snake -> x: %d\ty:%d\n", snake->body[0].x, snake->body[0].y);
        printf("Coords food-> x: %d\ty:%d\n", food->x, food->y);

        if (SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE) != 0){//make red color for snake
            ErrMessage("Error: Changing color for the render");
        }
        for (int i = 0; i<snake->size; i++){
            SDL_Rect snake_part = {snake->body[i].x, snake->body[i].y, PIXEL_UNIT-1, PIXEL_UNIT-1};
            if (SDL_RenderFillRect(renderer, &snake_part) != 0){
                ErrMessage("Error: Impossible to color a rectangle");
            }
        }

        if (checkCollision(snake)){
            printf("Game Over!\nScore: %d", score);
            SDL_RenderClear(renderer);
            clearFood(food);
            clearSnake(snake);
            break;
        }

        if (snake->body[0].x == food->x && snake->body[0].y == food->y){
            eat(snake);
            score++;
            food = initFood();
        }
        if (SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE) != 0){//make red color for food
            ErrMessage("Error: Changing color for the render");
        }
        SDL_Rect food_rec = {food->x, food->y, PIXEL_UNIT, PIXEL_UNIT};
        if (SDL_RenderFillRect(renderer, &food_rec) != 0){
                ErrMessage("Error: Impossible to color a rectangle");
        }
        unsigned int frameLimit = SDL_GetTicks()+FRAME_RATE;
        FPSLimit(frameLimit);
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_UP:
                            snake->movement = North;
                            continue;
                        case SDLK_DOWN:
                            snake->movement = South;
                            continue;
                        case SDLK_RIGHT:
                            snake->movement = East;
                            continue;
                        case SDLK_LEFT:
                            snake->movement = West;
                            continue;
                        default: //previous direction
                            continue;
                    }
                case SDL_QUIT:
                    running = SDL_FALSE;
                    clearSnake(snake);
                    clearFood(food);
                    break;
                default:
                    break;
            }
        }
        frameLimit = SDL_GetTicks()+FRAME_RATE;
        FPSLimit(frameLimit);
        SDL_RenderPresent(renderer);
        if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0){//make red color for snake
            ErrMessage("Error: Changing color for the render");
        }
        SDL_RenderClear(renderer);
        move(snake);
    }
    
    
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

void ErrMessage(char message[]){
    SDL_Log("%s -> %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void FPSLimit(unsigned int limit){
    unsigned int ticks= SDL_GetTicks();
    if (limit < ticks ){
        return;
    }
    else if (limit > ticks+FRAME_RATE){
        SDL_Delay(FRAME_RATE);
    }

    else{
        SDL_Delay(limit-ticks);
    }
}