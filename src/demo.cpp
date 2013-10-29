#include "SDL2/SDL.h"

#include "dot_factory.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        puts("Usage: dot_factory <elements> <classes>");
        exit(EXIT_FAILURE);
    }

    int dots = atoi(argv[1]);
    int classes = atoi(argv[2]);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_Window *window = SDL_CreateWindow("Bunch of dots v0.1",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        0, 0,
        SDL_WINDOW_FULLSCREEN_DESKTOP |
        SDL_WINDOW_BORDERLESS
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED
    );

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    DotFactory *demo = new DotFactory(dots, 1920, 1080);
    // demo->KMeans(classes, renderer);
    demo->MaxMin(renderer);

    // Event loop
    bool out = false;
    SDL_Event event;
    while ((!out) && (SDL_WaitEvent(&event)))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                out = true;
                break;

            case SDL_KEYDOWN:
                out = true;
                break;

            default:
                break;
        }

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
