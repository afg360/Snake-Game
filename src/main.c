#include <stdio.h>
#include <stdlib.h>
#include <Food.h>
#include <rendering.h>
#include <Constants.h>

int main(int argc, char *argv[]){
    setup();
    
    SDL_Window *window = init_window();
    SDL_Renderer *renderer = init_renderer(window);

    int running = 1;
    enum MENU state = menu;
    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    unsigned const int desired_delta = 1000 / MENU_RATE;
    unsigned int game_frames;

    while (running){
        unsigned int start = SDL_GetTicks();
        int mouse_x, mouse_y;
        int clicked = 0;
        switch (state){
            case menu:
                main_menu(renderer, &mouse_x, &mouse_y, cursor, &state, &running);
                break;
            case level:
                level_menu(renderer, &game_frames, &mouse_x, &mouse_y, cursor, &running, &state);
                break;
            case game:
                game_loop(renderer, &running, game_frames, &state);
                break;
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
        FPSLimit(start, desired_delta);
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_FreeCursor(cursor);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
