#include "goal.h"

void init_goal(Goal *goal, SDL_Renderer *renderer, const char *img_path, int x, int y) {
    SDL_Surface *surf = SDL_LoadBMP(img_path);
    goal->texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    goal->x = x;
    goal->y = y;
}

void render_goal(Goal *goal, SDL_Renderer *renderer) {
    SDL_Rect dst = {goal->x, goal->y, 48, 48};
    SDL_RenderCopy(renderer, goal->texture, NULL, &dst);
}

int check_goal_reached(Goal *goal, int px, int py) {
    SDL_Rect goal_rect = {goal->x, goal->y, 48, 48};
    SDL_Rect player_rect = {px, py, 48, 48};
    return SDL_HasIntersection(&goal_rect, &player_rect);
}

void free_goal(Goal *goal) {
    SDL_DestroyTexture(goal->texture);
}