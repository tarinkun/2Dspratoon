#include "staffroll.h"
#include <SDL_ttf.h>
#include <string.h>
#include <unistd.h>

void show_staffroll(SDL_Renderer *renderer) {
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 32);
    if (!font) return;
    const char* staff[] = {
        "Splatoon 2D Vita",
        "",
        "開発者: tarinkun",
        "協力: GitHub Copilot",
        "",
        "Special Thanks:",
        "Nintendo & Sony",
        "VitaSDK, SDL2, and Open Source Community",
        "",
        "Thank you for playing!",
        NULL
    };

    int staff_len = 0;
    while (staff[staff_len]) staff_len++;

    int window_w = 960, window_h = 544;
    int y_offset = window_h;

    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < staff_len; i++) {
            SDL_Color color = {255, 255, 255, 255};
            SDL_Surface* text_surf = TTF_RenderUTF8_Blended(font, staff[i], color);
            SDL_Texture* text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);
            int tw = text_surf->w, th = text_surf->h;
            SDL_Rect dst = { (window_w-tw)/2, y_offset + i*60, tw, th };
            SDL_RenderCopy(renderer, text_tex, NULL, &dst);
            SDL_FreeSurface(text_surf);
            SDL_DestroyTexture(text_tex);
        }
        SDL_RenderPresent(renderer);

        y_offset -= 2;

        if (y_offset + staff_len*60 < 0) {
            SDL_Delay(1500);
            running = 0;
        }

        SDL_Delay(16);
    }
    TTF_CloseFont(font);
}