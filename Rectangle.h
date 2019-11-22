#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "SDL_Plotter.h"

class Rectangle {
private:
    int x, y;
    int width, height;

public:
    explicit Rectangle(int x = 0, int y = 0, int width = 0, int height = 0);
    void draw(SDL_Plotter& g);
};

#endif // RECTANGLE_H
