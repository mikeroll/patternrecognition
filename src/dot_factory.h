#ifndef DOT_FACTORY_H
#define DOT_FACTORY_H

#include "SDL2/SDL.h"
#include <vector>

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
    std::vector< int > members;
};

class DotFactory
{
    int screen_w, screen_h;
    int n, class_count;
    std::vector< Dot > dots;
    std::vector< DotClass > dot_classes;

    void Populate();
    void ChooseKernels();
    void CreateClasses();
    void DrawKernel(SDL_Renderer *renderer, int i);

public:
    DotFactory(int n, int class_count, int w, int h);
    ~DotFactory();
    void Distribute();
    void Draw(SDL_Renderer *renderer);
    Dot operator[](int i) { return dots[i]; }
};

#endif

