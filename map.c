#include "map.h"

void load_map(Map *map, SDL_Renderer *renderer, const char *img_path) {
    SDL_Surface *surf = SDL_LoadBMP(img_path);
    map->surface = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_ARGB8888, 0);
    map->texture = SDL_CreateTextureFromSurface(renderer, map->surface);
    SDL_FreeSurface(surf);
}

void spread_ink_on_map(Map *map, int x, int y, int w, int h, int type) {
    SDL_LockSurface(map->surface);
    Uint32 *pixels = (Uint32*)map->surface->pixels;
    int pitch = map->surface->pitch / 4;
    Uint32 color;
    if (type == 0)
        color = SDL_MapRGBA(map->surface->format, 0,200,255,255); // プレイヤー（シアン）
    else
        color = SDL_MapRGBA(map->surface->format, 255,60,60,255); // 敵インク（赤）
    for (int j = y; j < y + h; j++) {
        for (int i = x; i < x + w; i++) {
            if (i >= 0 && i < map->surface->w && j >= 0 && j < map->surface->h) {
                pixels[j * pitch + i] = color;
            }
        }
    }
    SDL_UnlockSurface(map->surface);
}

int is_player_ink(Map *map, int x, int y) {
    Uint32 *pixels = (Uint32*)map->surface->pixels;
    int pitch = map->surface->pitch / 4;
    if (x >= 0 && x < map->surface->w && y >= 0 && y < map->surface->h) {
        Uint32 color = pixels[y * pitch + x];
        Uint8 r, g, b, a;
        SDL_GetRGBA(color, map->surface->format, &r, &g, &b, &a);
        if (r < 10 && g > 180 && b > 200) return 1;
    }
    return 0;
}

int is_enemy_ink(Map *map, int x, int y) {
    Uint32 *pixels = (Uint32*)map->surface->pixels;
    int pitch = map->surface->pitch / 4;
    if (x >= 0 && x < map->surface->w && y >= 0 && y < map->surface->h) {
        Uint32 color = pixels[y * pitch + x];
        Uint8 r, g, b, a;
        SDL_GetRGBA(color, map->surface->format, &r, &g, &b, &a);
        if (r > 180 && g < 80 && b < 80) return 1;
    }
    return 0;
}

void render_map(Map *map, SDL_Renderer *renderer) {
    SDL_UpdateTexture(map->texture, NULL, map->surface->pixels, map->surface->pitch);
    SDL_Rect dst = {0, 0, 960, 544};
    SDL_RenderCopy(renderer, map->texture, NULL, &dst);
}

void free_map(Map *map) {
    SDL_FreeSurface(map->surface);
    SDL_DestroyTexture(map->texture);
}