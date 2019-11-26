#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "SDL_Plotter.h"
#include "Color.h"

class Rectangle {
private:
    int x, y;
    int width, height;
    Color outlineColor;
    int outlineThickness;
    bool hasFill;
    Color fillColor;

public:
    explicit Rectangle(int x = 0, int y = 0, int width = 0, int height = 0,
            Color outlineColor = Color::BLACK(), int outlineThickness = 1, bool fill = false,
            Color fillColor = Color::BLACK())
     : x{x}, y{y}, width{width}, height{height}, outlineColor{outlineColor},
     hasFill{fill}, fillColor{fillColor}, outlineThickness{outlineThickness}
    {

    }

    void draw(SDL_Plotter& g);

    int getX() const { return x; }
    void setX(int x) { this->x = x; }

    int getY() const { return y; }
    void setY(int y) { this->y = y; }

    int getWidth() const { return width; }
    void setWidth(int width) { this->width = width; }

    int getHeight() const { return this->height; }
    void setHeight(int height) { this->height = height; }
};

#endif // RECTANGLE_H
