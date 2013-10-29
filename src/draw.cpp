#include "SDL2/SDL.h"

#include "dot_factory.h"

void DotFactory::Draw(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    SDL_RenderClear(renderer);
    for (int i = 0; i < class_count; ++i)
    {
        DrawClass(renderer, i);
        DrawKernel(renderer, i);
    }
    SDL_RenderPresent(renderer);
}

void DotFactory::DrawClass(SDL_Renderer *renderer, int i)
{
    SDL_Point *dots_storage = (SDL_Point *)(&(classes[i].members[0]));
    SDL_Color *cl = &(classes[i].color);
    SDL_SetRenderDrawColor(renderer, cl->r, cl->g, cl->b, cl->a);
    SDL_RenderDrawPoints(renderer, dots_storage,
        classes[i].members.size());
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 55);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, kern.x, kern.y-delta+1, kern.x, kern.y+delta-1);
    SDL_RenderDrawLine(renderer, kern.x-delta+1, kern.y, kern.x+delta-1, kern.y);
}
