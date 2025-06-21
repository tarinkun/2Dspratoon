#include "player.h"

#define INK_COST 5
#define INK_REFILL_RATE 2
#define INK_REFILL_RATE_SUBMERGED 6
#define HP_DAMAGE_RATE 1
#define HP_RECOVER_RATE 2

void init_player(Player *player, SDL_Renderer *renderer, const char *img_path) {
    SDL_Surface *surf = SDL_LoadBMP(img_path);
    player->texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    player->x = 100;
    player->y = 400;
    player->ink.max = 100;
    player->ink.current = 100;
    player->is_refilling = 0;
    player->is_submerged = 0;
    player->hp_max = 100;
    player->hp = 100;
}

void refill_ink(Player *player, int rate) {
    if (player->ink.current < player->ink.max) {
        player->ink.current += rate;
        if (player->ink.current > player->ink.max) player->ink.current = player->ink.max;
    }
}

int try_spread_ink(Player *player, Map *map) {
    if (player->ink.current < INK_COST) return 0;
    spread_ink_on_map(map, player->x + 12, player->y + 40, 24, 16, 0); // 0:自分インク
    player->ink.current -= INK_COST;
    return 1;
}

void update_submerged_state(Player *player, Map *map) {
    int px = player->x + 24;
    int py = player->y + 44;
    player->is_submerged = is_player_ink(map, px, py);
}

void handle_player_input(Player *player, const Uint8 *keystate, Map *map) {
    if (keystate[SDL_SCANCODE_LEFT])  player->x -= 4;
    if (keystate[SDL_SCANCODE_RIGHT]) player->x += 4;
    if (keystate[SDL_SCANCODE_UP])    player->y -= 4;
    if (keystate[SDL_SCANCODE_DOWN])  player->y += 4;

    update_submerged_state(player, map);

    if (keystate[SDL_SCANCODE_SPACE]) {
        try_spread_ink(player, map);
    }

    // 潜伏: 下キー長押しで潜伏+回復
    int want_submerge = keystate[SDL_SCANCODE_DOWN];
    player->is_refilling = 0;
    if (player->is_submerged && want_submerge) {
        refill_ink(player, INK_REFILL_RATE_SUBMERGED);
    } else if (keystate[SDL_SCANCODE_B]) {
        player->is_refilling = 1;
        refill_ink(player, INK_REFILL_RATE);
    }
}

void update_player_hp(Player *player, Map *map) {
    int px = player->x + 24;
    int py = player->y + 44;
    if (is_enemy_ink(map, px, py)) {
        player->hp -= HP_DAMAGE_RATE;
        if (player->hp < 0) player->hp = 0;
    }
    if (player->is_submerged) {
        player->hp += HP_RECOVER_RATE;
        if (player->hp > player->hp_max) player->hp = player->hp_max;
    }
}

void render_player(Player *player, SDL_Renderer *renderer) {
    SDL_Rect dst = {player->x, player->y, 48, 48};
    SDL_RenderCopy(renderer, player->texture, NULL, &dst);

    // インクゲージを画面右上に表示
    int gauge_width = 100;
    int gauge_height = 10;
    int margin = 10;
    SDL_Rect inkbar_bg = {960 - gauge_width - margin, margin, gauge_width, gauge_height};
    SDL_Rect inkbar_fg = {960 - gauge_width - margin, margin, player->ink.current, gauge_height};
    SDL_SetRenderDrawColor(renderer, 80,80,80,255); SDL_RenderFillRect(renderer, &inkbar_bg);
    SDL_SetRenderDrawColor(renderer, 0,180,255,255); SDL_RenderFillRect(renderer, &inkbar_fg);

    // HPゲージをその下に表示
    SDL_Rect hpbar_bg = {960 - gauge_width - margin, margin + gauge_height + 5, gauge_width, 10};
    SDL_Rect hpbar_fg = {960 - gauge_width - margin, margin + gauge_height + 5, (player->hp * gauge_width)/player->hp_max, 10};
    SDL_SetRenderDrawColor(renderer, 80,80,80,255); SDL_RenderFillRect(renderer, &hpbar_bg);
    SDL_SetRenderDrawColor(renderer, 255,60,60,255); SDL_RenderFillRect(renderer, &hpbar_fg);

    SDL_SetRenderDrawColor(renderer, 255,255,255,255); // reset
}

void free_player(Player *player) {
    SDL_DestroyTexture(player->texture);
}
