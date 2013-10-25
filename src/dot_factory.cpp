#include <ctime>
#include "SDL2/SDL.h"

#include "dot_factory.h"

DotFactory::DotFactory(int n, int w, int h) : n(n), w(w), h(h)
{
    srand(time(NULL));
    CreatePool();
}

void DotFactory::CreatePool()
{
    dots.resize(n);
    for (int i = 0; i < n; ++i)
    {
        dots[i].x = rand() % w;
        dots[i].y = rand() % h;
    }
}

void DotFactory::CreateClasses(int k)
{
    class_count = k;
    classes.resize(class_count);
    for (int i = 0; i < class_count; ++i)
    {
        classes[i].kernel = dots[i];
        classes[i].color.r = rand() % 128 + 127;
        classes[i].color.g = rand() % 256;
        classes[i].color.b = rand() % 256;
        classes[i].color.a = 255;
    }
}

void DotFactory::Redistribute()
{

    for (int i = 0; i < class_count; ++i)
        classes[i].members.clear();

    for (int i = 0; i < dots.size(); ++i)
    {
        int min = h * h + w * w + 1;
        int candidate;
        for (int k = 0; k < class_count; ++k)
        {
            Dot kern = classes[k].kernel;
            int dx = kern.x - dots[i].x;
            int dy = kern.y - dots[i].y;
            int newmin = dx * dx + dy * dy;
            if (newmin < min)
            {
                min = newmin;
                candidate = k;
            }
        }
        classes[candidate].members.push_back(dots[i]);
    }
}

bool DotFactory::Normalize()
{
    bool trigger = false;
    for (int k = 0; k < class_count; ++k)
    {
        Dot new_kernel;
        long min_deviation = (w * w + h * h) * classes[k].members.size();
        for (int i = 0; i < classes[k].members.size(); ++i)
        {
            long deviation = 0;
            for (int j = 0; j < classes[k].members.size(); ++j)
            {
                int dx = classes[k].members[j].x - classes[k].members[i].x;
                int dy = classes[k].members[j].y - classes[k].members[i].y;
                deviation += dx * dx + dy * dy;
            }
            if (deviation < min_deviation)
            {
                min_deviation = deviation;
                new_kernel = classes[k].members[i];
            }
        }
        if ((classes[k].kernel.x != new_kernel.x) &&
            (classes[k].kernel.y != new_kernel.y))
        {
            classes[k].kernel = new_kernel;
            trigger = true;
        }
    }
    return trigger;
}

void DotFactory::Draw(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    for (int i = 0; i < class_count; ++i)
    {
        SDL_Point *dots_storage = (SDL_Point *)(&(classes[i].members[0]));
        SDL_Color *cl = &(classes[i].color);
        SDL_SetRenderDrawColor(renderer, cl->r, cl->g, cl->b, cl->a);
        SDL_RenderDrawPoints(renderer, dots_storage,
            classes[i].members.size());
        DrawKernel(renderer, i);
    }
    SDL_RenderPresent(renderer);
}

void DotFactory::DrawKernel(SDL_Renderer *renderer, int i)
{
    int delta = 7;
    Dot kern = classes[i].kernel;
    SDL_Rect rect;
    rect.x = kern.x - delta;
    rect.y = kern.y - delta;
    rect.w = delta * 2 + 1;
    rect.h = rect.w;
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, kern.x, kern.y-delta+1, kern.x, kern.y+delta-1);
    SDL_RenderDrawLine(renderer, kern.x-delta+1, kern.y, kern.x+delta-1, kern.y);
}

void DotFactory::KMeans(int k, SDL_Renderer *renderer)
{
    CreateClasses(k);
    do
    {
        Redistribute();
        if (renderer != NULL)
            Draw(renderer);
    } while (Normalize());
}
