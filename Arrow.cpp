#include <cmath>
#include "Arrow.h"
#include "Rectangle.h"

void Arrow::draw(SDL_Plotter &g) {
    int xDiff = toX - fromX;
    int yDiff = toY - fromY;
    float length = sqrt(xDiff*xDiff + yDiff*yDiff);
    // TODO: How many samples?
    int numSamples = (int)(length);
    float curX = (float)fromX;
    float curY = (float)fromY;
    float dX = (float)(xDiff) / (float)(numSamples);
    float dY = (float)(yDiff) / (float)(numSamples);
    bool inBounds = (int)curX >= -thickness
        && (int)curX + thickness < g.getCol()
        && (int)curY >= -thickness
        && (int)curY + thickness < g.getRow();
    for (int i = 0; i < numSamples && inBounds; ++i) {
        // TODO: Optimize this if needed
        Rectangle rect{(int)curX, (int)curY, thickness, thickness, 0, Color::BLACK(), true, color};
        rect.draw(g);
        curX += dX;
        curY += dY;
        inBounds = (int)curX >= -thickness
           && (int)curX + thickness < g.getCol()
           && (int)curY >= -thickness
           && (int)curY + thickness < g.getRow();
    }
}