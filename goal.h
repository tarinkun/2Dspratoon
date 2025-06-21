#pragma once
#include <SDL.h>

typedef struct {
    int x, y;
    SDL_Texture *texture;
} Goal;

void init_goal(Goal *goal, SDL_Renderer *renderer, const char *img_path, int x, int y);
void render_goal(Goal *goal, SDL_Renderer *renderer);
int check_goal_reached(Goal *goal, int px, int py);
void free_goal(Goal *goal);