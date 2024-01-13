#include <stdio.h>
#include <stdlib.h>
#include <food.h>
#include <rendering.h>
#include <constants.h>
//#include <options.h>

int main(int argc, char *argv[]){
    setup();
    
    SDL_Window *window = init_window();
    SDL_Renderer *renderer = init_renderer(window);

    int running = 1;
    enum MENU state = menu;
    High_Scores scores = check_score();
    printf("%d, %d, %d\n", scores.easy, scores.norm, scores.hard);
    SDL_Cursor *cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    unsigned const int desired_delta = 1000 / MENU_RATE;
    enum DIFF difficulty;

    while (running){
        unsigned int start = SDL_GetTicks();
        int mouse_x, mouse_y;
        int clicked = 0;
        switch (state){
            case menu:
                main_menu(renderer, &mouse_x, &mouse_y, cursor, &state, &running);
                break;
            case level:
                level_menu(renderer, &difficulty, &mouse_x, &mouse_y, cursor, &running, &state);
                break;
            case game:
                game_loop(renderer, &running, difficulty, &state, &scores);
                break;
            //case pause:
            //    pause_loop(renderer, &running);
            //    break;
            case options:
                options_menu(renderer, &mouse_x, &mouse_y, cursor, &state, &running);
                break;
            case quit:
                running = 0;
                break;
            default:
                SDL_Log("An error occured... Unavailable menu item");
                running = 0;
                break;
        }
        //FPSLimit(start, desired_delta);
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_FreeCursor(cursor);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
