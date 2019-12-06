#ifndef LINE_H
#define LINE_H

#include "SDL_Plotter.h"
#include "Color.h"

class Arrow {
private:
    int fromX;
    int fromY;
    int toX;
    int toY;
    Color color;
    int thickness;

public:
    explicit Arrow(int fromX =  0, int fromY = 0, int toX = 0, int toY = 0, int thickness = 1, Color c = Color::BLACK())
     : fromX{fromX}, fromY{fromY}, toX{toX}, toY{toY}, color{c}, thickness{thickness}
    {

    }
    void draw(SDL_Plotter& g);

    int getFromX() const { return fromX; }

    void setFromX(int fromX) { this->fromX = fromX; }

    int getFromY() const { return fromY; }

    void setFromY(int fromY) { this->fromY = fromY; }

    int getToX() const { return toX; }

    void setToX(int toX) {this->toX = toX; }

    int getToY() const { return toY; }

    void setToY(int toY) { this->toY = toY; }

    static void drawArrow(SDL_Plotter &g, int fromX, int fromY, int toX, int toY, int thickness = 1, Color c = Color::BLACK());

    static void drawLine(SDL_Plotter &g, int fromX, int fromY, int toX, int toY, int thickness = 1, Color c = Color::BLACK());
};

#endif // LINE_H
