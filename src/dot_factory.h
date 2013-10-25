#ifndef DOT_FACTORY_H
#define DOT_FACTORY_H

#include "SDL2/SDL.h"
#include <vector>

struct Dot
{
    int x;
    int y;
};

struct DotClass
{
    SDL_Color color;
    Dot kernel;
    std::vector< Dot > members;
};

class DotFactory
{
    int w, h;
    int n, class_count;
    std::vector< DotClass > classes;
    std::vector< Dot > dots;

    // Utiltity methods
    void CreatePool();
    void ChooseKernels();
    void CreateClasses();
    void DrawKernel(SDL_Renderer *renderer, int i);

    // Algorithm routines
    void Redistribute();
    bool Normalize();

public:
    DotFactory(int n, int class_count, int w, int h);
    ~DotFactory();

    void Draw(SDL_Renderer *renderer);
    void KMeans(SDL_Renderer *renderer);
};

#endif
