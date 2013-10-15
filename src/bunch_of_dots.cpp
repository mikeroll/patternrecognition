#include "SDL2/SDL.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <vector>

using std::vector;

struct Dot
{
    int x;
    int y;
    int class_index;
    int is_kernel;
};

struct DotClass
{
    int index;
    SDL_Color color;
    int kernel;
    vector< int > members;
};

class DotFactory
{
    int screen_w, screen_h;
    int n, classes;
    vector< Dot > dots;
    vector< int > kernel_indexes;
    vector< SDL_Color > class_colors;

    void Populate();
    void ChooseKernels();
    void GenerateColors();
    void DrawKernel(SDL_Renderer *renderer, int i);

public:
    DotFactory(int n, int classes, int w, int h);
    ~DotFactory();
    void Distribute();
    void Draw(SDL_Renderer *renderer);
    Dot operator[](int i) { return dots[i]; }
};


DotFactory::DotFactory(int n, int classes, int w, int h) :n (n), classes(classes), screen_w(w), screen_h(h)
{
        srand(time(NULL));
        Populate();
        ChooseKernels();
        GenerateColors();
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

void DotFactory::ChooseKernels()
{
    kernel_indexes.resize(classes);
    for (int i = 0; i < classes; ++i)
    {
        kernel_indexes[i] = i;
        dots[i].is_kernel = true;
    }
}

void DotFactory::GenerateColors()
{
    class_colors.resize(classes);
    for (int i = 0; i < classes; ++i)
    {
        class_colors[i].r = rand() % 128 + 127;
        class_colors[i].g = rand() % 256;
        class_colors[i].b = rand() % 256;
        class_colors[i].a = 255;
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
        for (int j = 0; j < classes; ++j)
        {
            dx = (dots[i].x - dots[kernel_indexes[j]].x);
            dy = (dots[i].y - dots[kernel_indexes[j]].y);
            newmin = dx * dx + dy * dy;
            if (newmin < min)
            {
                min = newmin;
                min_index = j;
                dots[i].class_index = j;
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
            SDL_Color color = class_colors[dots[i].class_index];
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

    DotFactory *lol = new DotFactory(10000, 10, 1920, 1080);
    lol->Draw(renderer);
    lol->Distribute();
    lol->Draw(renderer);

    // Event loop
    bool out;
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