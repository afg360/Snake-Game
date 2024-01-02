#ifndef __RENDERING_H__
#define __RENDERING_H__

#include <Snake.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <Food.h>

enum MENU{
    menu, level, game, options, quit
};

void setup();
SDL_Window *init_window();
SDL_Renderer *init_renderer(SDL_Window *window);
TTF_Font *open_font(int ptsize);
SDL_Texture *create_font_texture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color);
void color_snake(Snake *snake, SDL_Renderer *renderer);
void color_food(Food *food, SDL_Renderer *renderer);
void check_event(Snake *snake, Food *food, SDL_Event *event, int *running);

void options_menu(SDL_Renderer *renderer, SDL_Event event, int *mouse_x, int *mouse_y, 
                int *clicked, SDL_Cursor *cursor, enum MENU *choice, int *running);
void main_menu(SDL_Renderer *renderer, SDL_Texture *title_texture, SDL_Texture *play_selected_texture,
                SDL_Texture *play_unselected_texture, SDL_Texture *options_selected_texture, SDL_Texture *options_unselected_texture, 
                SDL_Rect play_rect, SDL_Rect options_rect, SDL_Event event, int *mouse_x, int *mouse_y, SDL_Cursor *cursor,
                enum MENU *choice, int *running);
void level_menu(SDL_Renderer *renderer, unsigned int *game_frames, 
                int *mouse_x, int *mouse_y, SDL_Cursor *cursor, int *running, enum MENU *state);
void game_loop(SDL_Renderer *renderer, int *running, unsigned const int frames, enum MENU *state);
void ErrMessage(const char *message);
void FPSLimit (unsigned int limit, unsigned const int desired_delta);

#endif