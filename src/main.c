#include <stdio.h>
#include <stdlib.h>
#include <Food.h>
#include <rendering.h>
#include <Constants.h>

int main(int argv, char *arcg[]){
    setup();
    TTF_Font *font = open_font(20);
    //TTF_Font *option_font = open_font(20);
    
    SDL_Window *window = init_window();
    SDL_Renderer *renderer = init_renderer(window);

    SDL_Color title_color = {0, 220, 80};
    SDL_Color unselected_color = {255, 0, 0};
    SDL_Color selected_color = {0, 0, 80};

    SDL_Texture *title_texture = create_font_texture(renderer, font, "Snake", title_color);
    SDL_Texture *play_unselected_texture = create_font_texture(renderer, font, "Play", unselected_color);
    SDL_Texture *play_selected_texture = create_font_texture(renderer, font, "Play", selected_color);
    SDL_Texture *options_unselected_texture = create_font_texture(renderer, font, "Options", unselected_color);
    SDL_Texture *options_selected_texture = create_font_texture(renderer, font, "Options", selected_color);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_Rect title_location = {WIDTH / 4, HEIGHT / 11, BOX_w * 2, BOX_h * 2};
    SDL_Rect play_rect = {WIDTH * 3 / 8, HEIGHT * 0.5, 
                    BOX_w, BOX_h};
    SDL_Rect options_rect = {WIDTH * 3 / 8, HEIGHT * 8 / 11, 
                    BOX_w, BOX_h};

    //SDL_RenderDrawRect(renderer, &play_rect);
    //SDL_RenderDrawRect(renderer, &options_rect);
    SDL_RenderCopy(renderer, title_texture, NULL, &title_location);
    SDL_RenderCopy(renderer, play_unselected_texture, NULL, &play_rect);
    SDL_RenderCopy(renderer, options_unselected_texture, NULL, &options_rect);
    int running = 1;
    enum MENU state = menu;
    SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    unsigned const int desired_delta = 1000 / MENU_RATE;
    unsigned int game_frames;

    while (running){
        unsigned int start = SDL_GetTicks();
        int mouse_x, mouse_y;
        int clicked = 0;
        SDL_Event event;
        switch (state){
            case menu:
                main_menu(window, renderer, title_texture, play_selected_texture, play_unselected_texture, options_selected_texture,
                options_unselected_texture, play_rect, options_rect, event, &mouse_x, &mouse_y, cursor, &state, &running);
                break;
            case level:
                level_menu(window, renderer, &game_frames, &mouse_x, &mouse_y, cursor, &running, &state);
                break;
            case game:
                game_loop(renderer, &running, MED_RATE);
                break;
            case options:
                options_menu(window, renderer, event, &mouse_x, &mouse_y, &clicked, cursor, &state, &running);
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
    
    SDL_DestroyTexture(title_texture);
    SDL_DestroyTexture(play_selected_texture);
    SDL_DestroyTexture(play_unselected_texture);
    SDL_DestroyTexture(options_selected_texture);
    SDL_DestroyTexture(options_unselected_texture);
    SDL_DestroyRenderer(renderer);
    SDL_FreeCursor(cursor);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
