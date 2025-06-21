#pragma once
#include <SDL.h>

typedef struct {
    SDL_Texture *texture;
    SDL_Surface *surface;
} Map;

void load_map(Map *map, SDL_Renderer *renderer, const char *img_path);
void render_map(Map *map, SDL_Renderer *renderer);
void free_map(Map *map);
void spread_ink_on_map(Map *map, int x, int y, int w, int h, int type); // type: 0=player, 1=enemy
int is_player_ink(Map *map, int x, int y);
int is_enemy_ink(Map *map, int x, int y);