#include "SDL2/SDL.h"
#include <cstdlib>
#include <cstdio>
#include <vector>

class DotFactory
{
    void Populate();
    std::vector<int> dots;
    int screen_h, screen_w;
    int n;
    int kernels[];
public:
    DotFactory(int n, int classes, int w, int h) : dots(n * 2), screen_w(w), screen_h(h) { };
    int* operator[](int row) { return &dots[row * 2]; }
    ~DotFactory();
};

int main(int argc, char const *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    atexit(SDL_Quit);

    SDL_Window *screen = SDL_CreateWindow("Bunch of dots v0.1",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        0, 0,
        SDL_WINDOW_FULLSCREEN_DESKTOP
    );
    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );


    return 0;
}