#include <SDL.h>
#include <SDL_ttf.h>
#include "player.h"
#include "map.h"
#include "enemy.h"
#include "goal.h"
#include "staffroll.h"

#define ENEMY_MAX 3

enum GameState {
    GAME_PLAYING,
    GAME_STAFFROLL,
    GAME_EXIT
};

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *window = SDL_CreateWindow("Vita Splat2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 544, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Map map;
    load_map(&map, renderer, "assets/map.bmp");
    Player player;
    init_player(&player, renderer, "assets/player.bmp");

    Enemy enemies[ENEMY_MAX];
    for (int i = 0; i < ENEMY_MAX; i++) {
        init_enemy(&enemies[i], renderer, "assets/enemy.bmp", 200 + i*200, 400);
    }

    Goal goal;
    init_goal(&goal, renderer, "assets/goal.bmp", 860, 400);

    enum GameState state = GAME_PLAYING;
    int running = 1;
    SDL_Event event;

    while (state != GAME_EXIT) {
        if (state == GAME_PLAYING) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) { running = 0; state = GAME_EXIT; }
            }
            const Uint8* keystate = SDL_GetKeyboardState(NULL);
            handle_player_input(&player, keystate, &map);

            // HP減少・回復
            update_player_hp(&player, &map);

            // HP0でリスタート
            if (player.hp <= 0) {
                init_player(&player, renderer, "assets/player.bmp");
                load_map(&map, renderer, "assets/map.bmp");
                for (int i = 0; i < ENEMY_MAX; i++) {
                    init_enemy(&enemies[i], renderer, "assets/enemy.bmp", 200 + i*200, 400);
                }
                continue;
            }

            // ゴール判定
            if (check_goal_reached(&goal, player.x, player.y)) {
                state = GAME_STAFFROLL;
                continue;
            }

            for (int i = 0; i < ENEMY_MAX; i++) {
                update_enemy(&enemies[i], &map);
            }

            SDL_RenderClear(renderer);
            render_map(&map, renderer);
            render_goal(&goal, renderer);
            render_player(&player, renderer);
            for (int i = 0; i < ENEMY_MAX; i++) {
                render_enemy(&enemies[i], renderer);
            }
            SDL_RenderPresent(renderer);

            SDL_Delay(16);
        } else if (state == GAME_STAFFROLL) {
            show_staffroll(renderer);
            state = GAME_EXIT;
        }
    }

    free_player(&player);
    for (int i = 0; i < ENEMY_MAX; i++) { free_enemy(&enemies[i]); }
    free_goal(&goal);
    free_map(&map);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
