#ifndef BPLUS_TREE_SCREENADJUSTEDCOORDS_H
#define BPLUS_TREE_SCREENADJUSTEDCOORDS_H

#include "SDL_Plotter.h"

struct ScreenAdjustedCoords {
    int x;
    int y;
    int width;
    int height;
    int leftCutOff;
    int rightCutOff;
    int topCutOff;
    int bottomCutOff;

    ScreenAdjustedCoords(SDL_Plotter& g, int x, int y, int width, int height) {
        this->width = width;
        if (x < 0) {
            this->leftCutOff = -x;
            this->width -= this->leftCutOff;
        }
        else {
            this->leftCutOff = 0;
        }
        if (x + width > g.getCol()) {
            this->rightCutOff = x + width - g.getCol();
            this->width -= this->rightCutOff;
        }
        else {
            this->rightCutOff = 0;
        }

        this->height = height;
        if (y < 0) {
            this->topCutOff = -y;
            this->height -= this->topCutOff;
        }
        else {
            this->topCutOff = 0;
        }
        if (y + height > g.getRow()) {
            this->bottomCutOff = y + height - g.getRow();
            this->height -= bottomCutOff;
        }
        else {
            bottomCutOff = 0;
        }

        if (this->width < 0)
            this->width = 0;
        if (this->height < 0)
            this->height = 0;

        this->x = max(x, 0);
        this->y = max(y, 0);
    }
};

#endif //BPLUS_TREE_SCREENADJUSTEDCOORDS_H
