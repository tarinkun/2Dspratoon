#pragma once
#include <SDL.h>
#include "map.h"

typedef struct {
    int current;
    int max;
} InkTank;

typedef struct {
    int x, y;
    SDL_Texture *texture;
    InkTank ink;
    int is_refilling;
    int is_submerged;
    int hp;
    int hp_max;
} Player;

void init_player(Player *player, SDL_Renderer *renderer, const char *img_path);
void handle_player_input(Player *player, const Uint8 *keystate, Map *map);
void render_player(Player *player, SDL_Renderer *renderer);
void free_player(Player *player);
void update_submerged_state(Player *player, Map *map);
void refill_ink(Player *player, int rate);
int try_spread_ink(Player *player, Map *map);
void update_player_hp(Player *player, Map *map);