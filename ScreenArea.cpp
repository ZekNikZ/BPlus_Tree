#include "ScreenArea.h"

ScreenArea::ScreenArea(SDL_Plotter &g, int x, int y, int width, int height)  {
    this->_width = width;
    if (x < 0) {
        this->_leftCutOff = -x;
        this->_width -= this->_leftCutOff;
    }
    else {
        this->_leftCutOff = 0;
    }
    if (x + width > g.getCol()) {
        this->_rightCutOff = x + width - g.getCol();
        this->_width -= this->_rightCutOff;
    }
    else {
        this->_rightCutOff = 0;
    }

    this->_height = height;
    if (y < 0) {
        this->_topCutOff = -y;
        this->_height -= this->_topCutOff;
    }
    else {
        this->_topCutOff = 0;
    }
    if (y + height > g.getRow()) {
        this->_bottomCutOff = y + height - g.getRow();
        this->_height -= _bottomCutOff;
    }
    else {
        _bottomCutOff = 0;
    }

    if (this->_width < 0)
        this->_width = 0;
    if (this->_height < 0)
        this->_height = 0;

    this->_x = max(x, 0);
    this->_y = max(y, 0);
}