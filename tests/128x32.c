#include <stddef.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

#include "../femtogfx.h"

const size_t WIDTH = 128;
const size_t HEIGHT = 32;
// const size_t WIDTH = 64 * 5;
// const size_t HEIGHT = 64 * 5;

int main() {
    SDL_Init(0);
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_CreateWindowAndRenderer("ez_monochrome_graphics_test", WIDTH, HEIGHT, 0, &window, &renderer);

    FemtogfxCanvas canvas = femtogfx_create(WIDTH, HEIGHT);
    femtogfx_fill(canvas, true);
    femtogfx_draw_string(canvas, false, "ABCDEFG yoman!!!", (FemtogfxVec2) {.x = 32, .y = 5});
    femtogfx_invert(canvas);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN && (event.key.key == SDLK_Q || event.key.key == SDLK_ESCAPE)) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        for (size_t i = 0 ; i < WIDTH ; i++) {
            for (size_t j = 0 ; j < HEIGHT ; j++) {
                if (femtogfx_get_pixel(canvas, i, j)) {
                    SDL_RenderPoint(renderer, i, j);
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 18);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
