#ifndef LINE_H
#define LINE_H

#include "SDL_Plotter.h"

class Line {
private:
    int fromX;
    int fromY;
    int toX;
    int toY;

public:
    explicit Line(int fromX =  0, int fromY = 0, int toX = 0, int toY = 0);
    void draw(SDL_Plotter& g);
};

#endif // LINE_H