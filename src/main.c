#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <Snake.h>
#include <Food.h>
#include <Constants.h>
#include <utils.h>
 //pixel size of a square

void ErrMessage(char message[]);
void ColorSnake(Snake *snake, SDL_Renderer *renderer);
void FPSLimit (unsigned int limit);

int score = 0;
const int desired_delta = 1000 / FRAME_RATE;

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
    Snake *snake = spawn();
    Food *food = initFood();
    move(snake);
    SDL_bool running = SDL_TRUE;
    while (running){
        unsigned int loop_start = SDL_GetTicks();
        SDL_Event event;
        //printf("Coords snake -> x: %d\ty:%d\n", snake->body[0].x, snake->body[0].y);
        //printf("Coords food-> x: %d\ty:%d\n", food->x, food->y);

        ColorSnake(snake, renderer);

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
        FPSLimit(SDL_GetTicks());
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_UP:
                        case SDLK_w:
                            snake->movement = North;
                            continue;
                        case SDLK_DOWN:
                        case SDLK_s:
                            snake->movement = South;
                            continue;
                        case SDLK_RIGHT:
                        case SDLK_d:
                            snake->movement = East;
                            continue;
                        case SDLK_LEFT:
                        case SDLK_a:
                            snake->movement = West;
                            continue;
                        default: //previous direction
                            continue;
                    }
                case SDL_QUIT:
                    running = SDL_FALSE;
                    //SDL_RenderClear(renderer);
                    clearSnake(snake);
                    clearFood(food);
                    break;
                default:
                    break;
            }
        }
        FPSLimit(SDL_GetTicks());
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

void ColorSnake(Snake *snake, SDL_Renderer *renderer){
        if (SDL_SetRenderDrawColor(renderer, 0, 204, 0, SDL_ALPHA_OPAQUE) != 0){//make blue color for head
            ErrMessage("Error: Changing color for the render");
        }
        SDL_Rect snake_part = {snake->body[0].x, snake->body[0].y, PIXEL_UNIT-1, PIXEL_UNIT-1};
        if (SDL_RenderFillRect(renderer, &snake_part) != 0){
            ErrMessage("Error: Impossible to color a rectangle");
        }
        if (snake->size > 1){
            for (int i = 1; i<snake->size - 1; i++){
                if (SDL_SetRenderDrawColor(renderer, 0, max(153, (int) (306 - i) / (5 * i)), 5 * i, SDL_ALPHA_OPAQUE) != 0){//make blue color for head
                    ErrMessage("Error: Changing color for the render");
                }
                SDL_Rect snake_part = {snake->body[i].x, snake->body[i].y, PIXEL_UNIT-1, PIXEL_UNIT-1};
                if (SDL_RenderFillRect(renderer, &snake_part) != 0){
                    ErrMessage("Error: Impossible to color a rectangle");
                }
            }
            if (SDL_SetRenderDrawColor(renderer, 0, 102, 0, SDL_ALPHA_OPAQUE) != 0){//make blue color for head
                ErrMessage("Error: Changing color for the render");
            }
            SDL_Rect snake_part = {snake->body[snake->size-1].x, snake->body[snake->size-1].y, PIXEL_UNIT-1, PIXEL_UNIT-1};
            if (SDL_RenderFillRect(renderer, &snake_part) != 0){
                ErrMessage("Error: Impossible to color a rectangle");
            }
        }
}

void FPSLimit(unsigned int start){
    unsigned int delta = SDL_GetTicks() - start;
    if (delta < desired_delta){
        SDL_Delay(desired_delta - delta);
    }
}