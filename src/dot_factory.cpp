#include <ctime>
#include "SDL2/SDL.h"

#include "dot_factory.h"


DotFactory::DotFactory(int n, int class_count, int w, int h) :n (n), class_count(class_count), screen_w(w), screen_h(h)
{
        srand(time(NULL));
        Populate();
        CreateClasses();
}

void DotFactory::Populate()
{
    dots.resize(n);
    for (int i = 0; i < n; ++i)
    {
        dots[i].x = rand() % screen_w + 1;
        dots[i].y = rand() % screen_h + 1;
        dots[i].is_kernel = false;
        dots[i].class_index = -1;
    }
}

void DotFactory::CreateClasses()
{
    dot_classes.resize(class_count);
    for (int i = 0; i < class_count; ++i)
    {
        dots[i].is_kernel = true;
        dot_classes[i].kernel = i;

        dot_classes[i].color.r = rand() % 128 + 127;
        dot_classes[i].color.g = rand() % 256;
        dot_classes[i].color.b = rand() % 256;
        dot_classes[i].color.a = 255;
    }
}

void DotFactory::Distribute()
{
    int min;
    int newmin;
    int min_index;
    int dx, dy;
    for (int i = 0; i < n; ++i)
    {
        min = screen_h * screen_h + screen_w * screen_w;
        for (int j = 0; j < class_count; ++j)
        {
            dx = (dots[i].x - dots[dot_classes[j].kernel].x);
            dy = (dots[i].y - dots[dot_classes[j].kernel].y);
            newmin = dx * dx + dy * dy;
            if (newmin < min)
            {
                min = newmin;
                min_index = j;
                dots[i].class_index = j;
                dot_classes[j].members.push_back(i);
            }
        }
    }
}

void DotFactory::Draw(SDL_Renderer *renderer)
{
    for (int i = 0; i < n; ++i)
    {
        if (dots[i].class_index == -1)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        else
        {
            SDL_Color color = dot_classes[dots[i].class_index].color;
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        }
        if (dots[i].is_kernel)
        {
            DrawKernel(renderer, i);
        }
        else
        {
            SDL_RenderDrawPoint(renderer, dots[i].x, dots[i].y);
        }
    }
    SDL_RenderPresent(renderer);
}

void DotFactory::DrawKernel(SDL_Renderer *renderer, int i)
{
    int delta = 5;
    SDL_Rect rect;
    rect.x = dots[i].x - delta;
    rect.y = dots[i].y - delta;
    rect.w = delta * 2 + 1;
    rect.h = rect.w;
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, dots[i].x, dots[i].y-delta, dots[i].x, dots[i].y+delta);
    SDL_RenderDrawLine(renderer, dots[i].x-delta, dots[i].y, dots[i].x+delta, dots[i].y);
}


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
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    DotFactory *lol = new DotFactory(dots, classes, 1920, 1080);
    lol->Draw(renderer);
    lol->Distribute();
    lol->Draw(renderer);

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