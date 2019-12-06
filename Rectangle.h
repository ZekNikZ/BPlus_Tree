#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "SDL_Plotter.h"
#include "Color.h"

/**
 * Represents a visible rectangle at a location with or without an outline and fill.
 */
class Rectangle {
private:
    int x, y;
    int width, height;
    Color outlineColor;
    int outlineThickness;
    bool hasFill;
    Color fillColor;

public:
    /**
     * Creates a rectangle
     * @param x x-coordinate of the top-left of the rectangle
     * @param y y-coordinate of the top-left of the rectangle
     * @param width Width of the rectangle
     * @param height Height of the rectangle
     * @param outlineThickness Thickness of the rectangle's outline. Set to 0 for no outline.
     * @param outlineColor Color of the rectangle's outline.
     * @param fill Whether to fill the rectangle or not.
     * @param fillColor The color to fill the rectangle with. Ignored if fill=false.
     */
    explicit Rectangle(int x = 0, int y = 0, int width = 0, int height = 0,
            int outlineThickness = 1, Color outlineColor = Color::BLACK(), bool fill = false,
            Color fillColor = Color::BLACK())
     : x{x}, y{y}, width{width}, height{height}, outlineColor{outlineColor},
     hasFill{fill}, fillColor{fillColor}, outlineThickness{outlineThickness}
    {

    }

    /**
     * Draws the rectangle to a SDL_Plotter.
     * @param g The plotter to draw on.
     */
    void draw(SDL_Plotter& g);

    int getX() const { return x; }
    void setX(int x) { this->x = x; }

    int getY() const { return y; }
    void setY(int y) { this->y = y; }

    int getWidth() const { return width; }
    void setWidth(int width) { this->width = width; }

    int getHeight() const { return this->height; }
    void setHeight(int height) { this->height = height; }

    void shiftX(int dx) {x += dx;}
    void shiftY(int dy) {y += dy;}
    void stretchX(int dx) {width += dx;}
    void stretchY(int dy) {height += dy;}

    void setFill(bool fill) {hasFill = fill;}
    void setFill(Color color, bool fill=true) {fillColor = color; hasFill = fill;}

    void setOutline(int thickness) {outlineThickness = thickness; }
    void setOutline(Color color, int thickness) {outlineColor = color; outlineThickness = thickness; }
};

#endif // RECTANGLE_H
