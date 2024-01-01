#include <stdio.h>
#include <stdlib.h>
#include <rendering.h>
#include <Constants.h>
#include <utils.h>

void setup(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_Log("Error: Initialisation of SDL -> %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    if (TTF_Init() != 0){
        puts("Could not init ttf!");
        TTF_Quit();
        exit(EXIT_FAILURE);
    }
}

SDL_Window *init_window(){
    SDL_Window *window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (window == NULL) ErrMessage("Error: Window Creation");
    return window;
}

SDL_Renderer *init_renderer(SDL_Window *window){
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) ErrMessage("Error: Renderer Creation");
}

TTF_Font *open_font(int ptsize){
    TTF_Font *font = TTF_OpenFont("./assets/ka1.ttf", ptsize);
    if (font == NULL) ErrMessage("Error: Renderer Creation");
    return font;
}

SDL_Texture *create_font_texture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color){
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (surface == NULL) ErrMessage("Error: Font Surface Creation");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) ErrMessage("Error: Font Texture Creation");
    return texture;
}

void color_snake(Snake *snake, SDL_Renderer *renderer){
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

void color_food(Food *food, SDL_Renderer *renderer){
    if (SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE) != 0){//make red color for food
        ErrMessage("Error: Changing color for the render");
    }
    SDL_Rect food_rec = {food->x, food->y, PIXEL_UNIT, PIXEL_UNIT};
    if (SDL_RenderFillRect(renderer, &food_rec) != 0){
            ErrMessage("Error: Impossible to color a rectangle");
    }
}

void check_event(Snake *snake, Food *food, SDL_Event *event, int *running){
    while (SDL_PollEvent(event)){
        switch (event->type){
            case SDL_KEYDOWN:
                switch (event->key.keysym.sym){
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
                *running = 0;
                //SDL_RenderClear(renderer);
                clearSnake(snake);
                clearFood(food);
                break;
            default:
                break;
        }
    }
}

void options_menu(SDL_Window *window, SDL_Renderer *renderer, SDL_Event event, int *mouse_x, int *mouse_y, 
                int *clicked, enum MENU *choice, int *running){
    SDL_Rect title_rect = {WIDTH * 1 / 8, HEIGHT / 9, 
                    BOX_h, BOX_h};
    SDL_Rect play_rect = {WIDTH * 3 / 8, HEIGHT * 4 / 9, 
                    BOX_w, BOX_h};
    SDL_Rect options_rect = {WIDTH * 3 / 8, HEIGHT * 6 / 9, 
                    BOX_w, BOX_h};

    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            *running = 0;
        }
        else if (event.type == SDL_MOUSEMOTION){
            *mouse_x = event.motion.x;
            *mouse_y = event.motion.y;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN){
            *clicked = 1;
        }
    }
    if (*mouse_x >= WIDTH * 1 / 8 && *mouse_x <= WIDTH * 1 / 8 + BOX_h
        && *mouse_y <= HEIGHT * 1 / 8 + BOX_h && *mouse_y >= HEIGHT * 1 / 8){
        // need error message if not 0
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &title_rect);
        if (*clicked){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            *choice = menu;
        }
    }
    else{
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &title_rect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(renderer, &title_rect);
    }
}


void main_menu(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *title_texture, SDL_Texture *play_selected_texture,
                SDL_Texture *play_unselected_texture, SDL_Texture *options_selected_texture, SDL_Texture *options_unselected_texture, 
                SDL_Rect play_rect, SDL_Rect options_rect, SDL_Event event, int *mouse_x, int *mouse_y, int *clicked, enum MENU *choice, int *running){
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            SDL_DestroyRenderer(renderer);
            SDL_DestroyTexture(title_texture);
            SDL_DestroyTexture(play_unselected_texture);
            SDL_DestroyTexture(play_selected_texture);
            SDL_DestroyTexture(options_unselected_texture);
            SDL_DestroyTexture(options_selected_texture);
            SDL_DestroyWindow(window);
            SDL_Quit();
            *running = 0;
        }
        else if (event.type == SDL_MOUSEMOTION){
            *mouse_x = event.motion.x;
            *mouse_y = event.motion.y;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN){
            *clicked = 1;
        }
    }
    if (*mouse_x >= play_rect.x && *mouse_x <= play_rect.x + play_rect.w
        && *mouse_y <= play_rect.y + play_rect.h && *mouse_y >= play_rect.y){
        // need error message if not 0
        SDL_RenderFillRect(renderer, &play_rect);
        SDL_RenderCopy(renderer, play_selected_texture, NULL, &play_rect);
        if (*clicked){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            *choice = game;
        }
    }
    else if (*mouse_x >= options_rect.x && *mouse_x <= options_rect.x + options_rect.w 
            && *mouse_y <= options_rect.y + options_rect.h && *mouse_y >= options_rect.y){
        // need error message if not 0
        SDL_RenderFillRect(renderer, &options_rect);
        SDL_RenderCopy(renderer, options_selected_texture, NULL, &options_rect);
        if (*clicked){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            *choice = options;
        }
    }
    else{
        SDL_Rect title_location = {WIDTH / 4, HEIGHT / 11, BOX_w * 2, BOX_h * 2};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &play_rect);
        SDL_RenderFillRect(renderer, &options_rect);
        SDL_RenderCopy(renderer, title_texture, NULL, &title_location);
        SDL_RenderCopy(renderer, play_unselected_texture, NULL, &play_rect);
        SDL_RenderCopy(renderer, options_unselected_texture, NULL, &options_rect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        //SDL_RenderDrawRect(renderer, &play_rect);
        //SDL_RenderDrawRect(renderer, &options_rect);
    }
}

void game_loop(SDL_Renderer *renderer, int *running){
    Snake *snake = spawn();
    Food *food = initFood();
    unsigned const int desired_delta = 1000 / FRAME_RATE;
    int score = 0;
    move(snake);
    while (*running){
        unsigned int loop_start = SDL_GetTicks();

        SDL_Event event;
        color_snake(snake, renderer);

        if (checkCollision(snake)){
            printf("Game Over!\nScore: %d", score);
            SDL_RenderClear(renderer);
            clearFood(food);
            clearSnake(snake);
            *running = 0;
        }

        if (snake->body[0].x == food->x && snake->body[0].y == food->y){
            eat(snake);
            score++;
            food = initFood();
        }
        color_food(food, renderer);
        FPSLimit(SDL_GetTicks(), desired_delta);
        check_event(snake, food, &event, running);
        FPSLimit(SDL_GetTicks(), desired_delta);
        SDL_RenderPresent(renderer);
        if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0){//make red color for snake
            ErrMessage("Error: Changing color for the render");
        }
        SDL_RenderClear(renderer);
        move(snake);
    }
}

void ErrMessage(const char *message){
    SDL_Log("%s -> %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void FPSLimit(unsigned int start, const unsigned int desired_delta){
    unsigned int delta = SDL_GetTicks() - start;
    if (delta < desired_delta){
        SDL_Delay(desired_delta - delta);
    }
}