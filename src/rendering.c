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
    TTF_Font *font = TTF_OpenFont("./assets/fonts/ka1.ttf", ptsize);
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

//take as argument a fxn. fxn is the switch statement (this one is same, others are repeated)
void check_event(Snake *snake, Food *food, SDL_Event *event, int *running){
    while (SDL_PollEvent(event)){
        switch (event->type){
            case SDL_KEYDOWN:
                switch (event->key.keysym.sym){
                    case SDLK_UP:
                    case SDLK_w:
                    //we will see if we keep it
                        if (snake->movement != South || snake->size <= 2) snake->movement = North;
                        //snake->movement = North;, perhaps in hardcore mode
                        continue;
                    case SDLK_DOWN:
                    case SDLK_s:
                    //we will see if we keep it
                        if (snake->movement != North || snake->size <= 2) snake->movement = South;
                        //snake->movement = South;
                        continue;
                    case SDLK_RIGHT:
                    case SDLK_d:
                    //we will see if we keep it
                        if (snake->movement != West || snake->size <= 2) snake->movement = East;
                        //snake->movement = East;
                        continue;
                    case SDLK_LEFT:
                    case SDLK_a:
                    //we will see if we keep it
                        if (snake->movement != East || snake->size <= 2) snake->movement = West;
                        //snake->movement = West;
                        continue;
                    default: //previous direction
                        continue;
                }
            case SDL_QUIT:
                *running = 0;
                clearSnake(snake);
                clearFood(food);
                break;
            default:
                break;
        }
    }
}

//make a fxn that does the rects filling for us. takes as arg an array for
//SDL_rects, alongside color we want

//also make a fxn "clear rect" to make rect all color we want (black)

void options_menu(SDL_Renderer *renderer, int *mouse_x, int *mouse_y, 
                SDL_Cursor *cursor, enum MENU *choice, int *running){
    /* Add possibility to configure size of game grid/map*/
    SDL_Rect title_rect = {WIDTH / 8, HEIGHT / 9, 
                    BOX_h, BOX_h};
    SDL_Rect play_rect = {WIDTH * 3 / 8, HEIGHT * 4 / 9, 
                    BOX_w, BOX_h};
    SDL_Rect options_rect = {WIDTH * 3 / 8, HEIGHT * 6 / 9, 
                    BOX_w, BOX_h};

    TTF_Font *back_font = open_font(15);
    SDL_Color back_color = {255, 255, 100};
    SDL_Texture *back_texture = create_font_texture(renderer, back_font, "<=", back_color);
    TTF_CloseFont(back_font);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &title_rect);
    SDL_RenderCopy(renderer, back_texture, NULL, &options_rect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, &title_rect);

    unsigned const int desired_delta = 1000 / MENU_RATE;
    SDL_Event event;
    while(*running){
        unsigned int start = SDL_GetTicks();
        int clicked = 0;
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    *running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    clicked = 1;
                    break;
            }
        }
        SDL_GetMouseState(mouse_x, mouse_y);
        if (*mouse_x >= WIDTH * 1 / 8 && *mouse_x <= WIDTH * 1 / 8 + BOX_h
            && *mouse_y <= HEIGHT * 1 / 8 + BOX_h && *mouse_y >= HEIGHT * 1 / 8){
            // need error message if not 0
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &title_rect);
            SDL_SetCursor(cursor);
            if (clicked){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);
                *choice = menu;
                SDL_SetCursor(SDL_GetDefaultCursor());
                break;
            }
        }
        else{
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &title_rect);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(renderer, &title_rect);
            SDL_RenderCopy(renderer, back_texture, NULL, &options_rect);
            SDL_SetCursor(SDL_GetDefaultCursor());
        }
        FPSLimit(start, desired_delta);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(back_texture);
}


void main_menu(SDL_Renderer *renderer, int *mouse_x, int *mouse_y, SDL_Cursor *cursor, enum MENU *choice, int *running){
    TTF_Font *font = open_font(20);
    SDL_Rect title_location = {WIDTH / 4, HEIGHT / 11, BOX_w * 2, BOX_h * 2};
    SDL_Rect play_rect = {WIDTH * 3 / 8, HEIGHT * 0.5, 
                    BOX_w, BOX_h};
    SDL_Rect options_rect = {WIDTH * 3 / 8, HEIGHT * 8 / 11, 
                    BOX_w, BOX_h};

    SDL_Color title_color = {0, 220, 80};
    SDL_Color unselected_color = {255, 0, 0};
    SDL_Color selected_color = {0, 0, 80};

    SDL_Texture *title_texture = create_font_texture(renderer, font, "Snake", title_color);
    SDL_Texture *play_unselected_texture = create_font_texture(renderer, font, "Play", unselected_color);
    SDL_Texture *play_selected_texture = create_font_texture(renderer, font, "Play", selected_color);
    SDL_Texture *options_unselected_texture = create_font_texture(renderer, font, "Options", unselected_color);
    SDL_Texture *options_selected_texture = create_font_texture(renderer, font, "Options", selected_color);
    TTF_CloseFont(font);

    //SDL_Rect title_location = {WIDTH / 4, HEIGHT / 11, BOX_w * 2, BOX_h * 2};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &play_rect);
    SDL_RenderFillRect(renderer, &options_rect);
    SDL_RenderCopy(renderer, title_texture, NULL, &title_location);
    SDL_RenderCopy(renderer, play_unselected_texture, NULL, &play_rect);
    SDL_RenderCopy(renderer, options_unselected_texture, NULL, &options_rect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_SetCursor(SDL_GetDefaultCursor());

    unsigned const int desired_delta = 1000 / MENU_RATE;
    while (*running){
        unsigned int start = SDL_GetTicks();
        int clicked = 0;
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    SDL_DestroyTexture(title_texture);
                    SDL_DestroyTexture(play_selected_texture);
                    SDL_DestroyTexture(play_unselected_texture);
                    SDL_DestroyTexture(options_selected_texture);
                    SDL_DestroyTexture(options_unselected_texture);
                    *running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    clicked = 1;
                    break;
            }
        }
        SDL_GetMouseState(mouse_x, mouse_y);
        if (*mouse_x >= play_rect.x && *mouse_x <= play_rect.x + play_rect.w
            && *mouse_y <= play_rect.y + play_rect.h && *mouse_y >= play_rect.y){
            // need error message if not 0
            SDL_RenderFillRect(renderer, &play_rect);
            SDL_RenderCopy(renderer, play_selected_texture, NULL, &play_rect);
            SDL_SetCursor(cursor);
            if (clicked){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);
                SDL_SetCursor(SDL_GetDefaultCursor());
                *choice = level;
                break;
            }
        }
        else if (*mouse_x >= options_rect.x && *mouse_x <= options_rect.x + options_rect.w 
                && *mouse_y <= options_rect.y + options_rect.h && *mouse_y >= options_rect.y){
            // need error message if not 0
            SDL_RenderFillRect(renderer, &options_rect);
            SDL_RenderCopy(renderer, options_selected_texture, NULL, &options_rect);
            SDL_SetCursor(cursor);
            if (clicked){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);
                SDL_SetCursor(SDL_GetDefaultCursor());
                *choice = options;
                break;
            }
        }
        else{
            //SDL_Rect title_location = {WIDTH / 4, HEIGHT / 11, BOX_w * 2, BOX_h * 2};
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &play_rect);
            SDL_RenderFillRect(renderer, &options_rect);
            SDL_RenderCopy(renderer, title_texture, NULL, &title_location);
            SDL_RenderCopy(renderer, play_unselected_texture, NULL, &play_rect);
            SDL_RenderCopy(renderer, options_unselected_texture, NULL, &options_rect);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_SetCursor(SDL_GetDefaultCursor());
        }
        FPSLimit(start, desired_delta);
        SDL_RenderPresent(renderer);
    }
}

void level_menu(SDL_Renderer *renderer, unsigned int *game_frames, 
            int *mouse_x, int *mouse_y, SDL_Cursor *cursor, int *running, enum MENU *state){
    //bug -> once we comeback to level screen, since the first thing
    //is to go back at where the mouse clicked, it will never fully render the
    //other rects
    SDL_Rect easy = {WIDTH / 8, HEIGHT * 3 / 9, BOX_w * 2 / 4, BOX_w * 2 / 4};
    SDL_Rect medium = {WIDTH * 7 / 16, HEIGHT * 3 / 9, BOX_w * 2 / 4, BOX_w * 2 / 4};
    SDL_Rect hard = {WIDTH * 6 / 8, HEIGHT * 3 / 9, BOX_w * 2 / 4, BOX_w * 2 / 4};
    SDL_Rect rects[] = {easy, medium, hard};

    //we could separate different rendring things, and make these font rects
    //as structs
    SDL_Rect easy_font = {WIDTH / 8, HEIGHT * 4 / 9, BOX_w * 2 / 4, BOX_w * 2 / 4 * 1 / 3};
    SDL_Rect hard_font = {WIDTH * 6 / 8, HEIGHT * 4 / 9, BOX_w * 2 / 4, BOX_w * 2 / 4 * 1 / 3};
    SDL_Rect medium_font = {WIDTH * 7 / 16, HEIGHT * 4 / 9, BOX_w * 2 / 4, BOX_w * 2 / 4 * 1 / 3};
    SDL_Rect back = {WIDTH / 8, HEIGHT  / 9, BOX_h, BOX_h };
    SDL_Rect fonts[] = {easy_font, medium_font, hard_font, back};

    TTF_Font *font = open_font(10);
    SDL_Color color = {220, 220, 220};
    SDL_Texture *easy_texture = create_font_texture(renderer, font, "Easy", color);
    SDL_Texture *med_texture = create_font_texture(renderer, font, "Medium", color);
    SDL_Texture *hard_texture = create_font_texture(renderer, font, "Hard", color);
    TTF_CloseFont(font);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRects(renderer, fonts, 4);
    SDL_RenderCopy(renderer, easy_texture, NULL, &easy_font);
    SDL_RenderCopy(renderer, med_texture, NULL, &medium_font);
    SDL_RenderCopy(renderer, hard_texture, NULL, &hard_font);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawRect(renderer, &back);
    SDL_RenderDrawRects(renderer, rects, 3);
    SDL_SetCursor(SDL_GetDefaultCursor());
    //need to also get rid of rectangle if not there
    SDL_Event event;
    unsigned const int desired_delta = 1000 / MENU_RATE;

    while (*running){
        unsigned int start = SDL_GetTicks();
        int clicked = 0;
        while (SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    SDL_DestroyTexture(easy_texture);
                    SDL_DestroyTexture(med_texture);
                    SDL_DestroyTexture(hard_texture);
                    *running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    clicked = 1;
                    break;
            }
        }
        SDL_GetMouseState(mouse_x, mouse_y);
        if (*mouse_x >= easy.x && *mouse_x <= easy.x + easy.w
            && *mouse_y <= easy.y + easy.h && *mouse_y >= easy.y){
            // need error message if not 0
            SDL_SetRenderDrawColor(renderer, 190, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &easy_font);
            SDL_RenderCopy(renderer, easy_texture, NULL, &easy_font);
            SDL_SetCursor(cursor);
            if (clicked){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);
                *game_frames = EASY_RATE;
                *state = game;
                SDL_SetCursor(SDL_GetDefaultCursor());
                break;
            }
        }
        else if (*mouse_x >= medium.x && *mouse_x <= medium.x + medium.w 
                && *mouse_y <= medium.y + medium.h && *mouse_y >= medium.y){
            // need error message if not 0
            SDL_SetRenderDrawColor(renderer, 190, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &medium_font);
            SDL_RenderCopy(renderer, med_texture, NULL, &medium_font);
            SDL_SetCursor(cursor);
            if (clicked){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);
                *game_frames = MED_RATE;
                *state = game;
                SDL_SetCursor(SDL_GetDefaultCursor());
                break;
            }
        }
        else if (*mouse_x > hard.x && *mouse_x <= hard.x + hard.w 
                && *mouse_y <= hard.y + hard.h && *mouse_y >= hard.y){
            // need error message if not 0
            SDL_SetRenderDrawColor(renderer, 190, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &hard_font);
            SDL_RenderCopy(renderer, hard_texture, NULL, &hard_font);
            SDL_SetCursor(cursor);
            if (clicked){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);
                *game_frames = HARD_RATE;
                *state = game;
                SDL_SetCursor(SDL_GetDefaultCursor());
                break;
            }
        }
        else if (*mouse_x >= back.x && *mouse_x <= back.x + back.w 
                && *mouse_y <= back.y + back.h && *mouse_y >= back.y){
            // need error message if not 0
            SDL_SetRenderDrawColor(renderer, 190, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &back);
            SDL_SetCursor(cursor);
            if (clicked){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);
                *state = menu;
                SDL_SetCursor(SDL_GetDefaultCursor());
                break;
            }
        }
        else{
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRects(renderer, fonts, 4);
            SDL_RenderCopy(renderer, easy_texture, NULL, &easy_font);
            SDL_RenderCopy(renderer, med_texture, NULL, &medium_font);
            SDL_RenderCopy(renderer, hard_texture, NULL, &hard_font);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(renderer, &back);
            SDL_RenderDrawRects(renderer, rects, 3);
            SDL_SetCursor(SDL_GetDefaultCursor());
        }
        FPSLimit(start, desired_delta);
        SDL_RenderPresent(renderer);
    }
}

void game_loop(SDL_Renderer *renderer, int *running, unsigned int frames, enum MENU *state){
    Snake *snake = spawn();
    Food *food = initFood();
    unsigned const int desired_delta = 1000 / frames;
    int score = 0;
    move(snake);
    while (*running){
        SDL_Event event;
        color_snake(snake, renderer);

        if (checkCollision(snake)){
            SDL_Log("Game Over!\nScore: %d\n", score);
            clearFood(food);
            clearSnake(snake);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            *state = level;
            break;
        }

        if (snake->body[0].x == food->x && snake->body[0].y == food->y){
            eat(snake);
            score++;
            Food *old = food;
            food = initFood();
            clearFood(old);
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