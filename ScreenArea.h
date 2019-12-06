#ifndef BPLUS_TREE_SCREENAREA_H
#define BPLUS_TREE_SCREENAREA_H

#include "SDL_Plotter.h"

/**
 * Represents a area (x,y,width,height) that has been adjusted to fit inside
 * the screen, and the length cut off from each direction to adjust it.
 */
class ScreenArea {
public:
    /**
     * Adjusts the area to the given plotter.
     * \Postcondition The area with the top-left at (x,y) and the resulting
     *   width and height are safe to draw on-screen. The amount cut off
     *   on each side plus the width and height equal the original width and height.
     *   width >= 0.
     *   height >= 0.
     * @param g The plotter to adjust to.
     * @param x The x-coordinate of the top-left of the area.
     * @param y The y-coordinate of the top-right of the area.
     * @param width The width of the area.
     * @param height The height of the area.
     */
    ScreenArea(SDL_Plotter& g, int x, int y, int width, int height);

private:
// these have underscores so I can use them as function names
    int _x;
    int _y;
    int _width;
    int _height;
    int _leftCutOff;
    int _rightCutOff;
    int _topCutOff;
    int _bottomCutOff;
public:
    int x() const { return this->_x; }
    int left() const {return _x; }
    int y() const { return _y; }
    int top() const { return _y; }
    int width() const { return _width; }
    int height() const { return _height; }
    int right() const { return _x + _width - 1; }
    int bottom() const { return _y + _height - 1; }
    int leftCutOff() const { return _leftCutOff; }
    int rightCutOff() const { return _rightCutOff; }
    int topCutOff() const { return _topCutOff; }
    int bottomCutOff() const { return _bottomCutOff; }
    int horizontalCutOff() const { return _leftCutOff + _rightCutOff; }
    int verticalCutOff() const { return _topCutOff + _bottomCutOff; }
};

#endif //BPLUS_TREE_SCREENAREA_H
