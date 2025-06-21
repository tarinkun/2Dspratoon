#pragma once
#include <SDL.h>
#include "map.h"

typedef struct {
    int x, y;
    int dx, dy;
    SDL_Texture *texture;
    int alive;
    int ink_timer;
} Enemy;

void init_enemy(Enemy *enemy, SDL_Renderer *renderer, const char *img_path, int x, int y);
void update_enemy(Enemy *enemy, Map *map);
void render_enemy(Enemy *enemy, SDL_Renderer *renderer);
void free_enemy(Enemy *enemy);