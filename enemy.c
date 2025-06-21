#include "enemy.h"

void init_enemy(Enemy *enemy, SDL_Renderer *renderer, const char *img_path, int x, int y) {
    SDL_Surface *surf = SDL_LoadBMP(img_path);
    enemy->texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    enemy->x = x;
    enemy->y = y;
    enemy->dx = 2;
    enemy->dy = 0;
    enemy->alive = 1;
    enemy->ink_timer = 0;
}

void update_enemy(Enemy *enemy, Map *map) {
    if (!enemy->alive) return;
    enemy->x += enemy->dx;
    if (enemy->x < 0 || enemy->x > 900) enemy->dx = -enemy->dx;
    // 敵が移動しながらインクを塗る
    enemy->ink_timer++;
    if (enemy->ink_timer >= 16) {
        spread_ink_on_map(map, enemy->x + 12, enemy->y + 40, 24, 16, 1); // 1:敵インク
        enemy->ink_timer = 0;
    }
}

void render_enemy(Enemy *enemy, SDL_Renderer *renderer) {
    if (!enemy->alive) return;
    SDL_Rect dst = {enemy->x, enemy->y, 48, 48};
    SDL_RenderCopy(renderer, enemy->texture, NULL, &dst);
}

void free_enemy(Enemy *enemy) {
    SDL_DestroyTexture(enemy->texture);
}