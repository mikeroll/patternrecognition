#include <ctime>
#include "SDL2/SDL.h"

#include "dot_factory.h"

inline int DotFactory::Distance(Dot d1, Dot d2)
{
    return (d1.x - d2.x) * (d1.x - d2.x) +
           (d1.y - d2.y) * (d1.y - d2.y);
}

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

void DotFactory::Reset()
{
    classes.clear();
    class_count = 0;
}

void DotFactory::AddClass(Dot kernel)
{
    DotClass dc;
    dc.kernel = kernel;
    dc.color.r = rand() % 128 + 127;
    dc.color.g = rand() % 256;
    dc.color.b = rand() % 256;
    dc.color.a = 255;
    class_count++;
    classes.push_back(dc);
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
            int newmin = Distance(classes[k].kernel, dots[i]);
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
                deviation += Distance(classes[k].members[j], classes[k].members[i]);
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

bool DotFactory::FindNewKernel()
{
    int max_distance = 0;
    Dot farthest_dot;
    for (int k = 0; k < class_count; ++k)
    {
        for (int i = 0; i < classes[k].members.size(); ++i)
        {
            int distance = Distance(classes[k].kernel, classes[k].members[i]);
            if (distance > max_distance)
            {
                max_distance = distance;
                farthest_dot = classes[k].members[i];
            }
        }
    }

    int avg_kernel_spacing = 0;
    int pairs = 0;
    for (int i = 0; i < class_count-1; ++i)
    {
        for (int j = i + 1; j < class_count; ++j)
        {
            avg_kernel_spacing += Distance(classes[i].kernel, classes[j].kernel);
            pairs++;
        }
    }

    bool trigger = false;
    if (4 * pairs * max_distance >= avg_kernel_spacing)
    {
        AddClass(farthest_dot);
        trigger = true;
    }
    return trigger;
}

void DotFactory::KMeans(int k, SDL_Renderer *renderer)
{
    Reset();
    for (int i = 0; i < k; ++i)
    {
        AddClass(dots[i]);
    }
    do
    {
        Redistribute();
        if (renderer != NULL) Draw(renderer);
    } while (Normalize());
}

void DotFactory::MaxMin(SDL_Renderer *renderer)
{
    Reset();
    AddClass(dots[0]);
    do
    {
        Redistribute();
        if (renderer != NULL) Draw(renderer);
    } while(FindNewKernel());
    do
    {
        Redistribute();
        if (renderer != NULL) Draw(renderer);
    } while (Normalize());
}
