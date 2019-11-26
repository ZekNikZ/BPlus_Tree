#include "Rectangle.h"
#include "ScreenAdjustedCoords.h"


void Rectangle::draw(SDL_Plotter &g) {
    ScreenAdjustedCoords coords(g, x, y, width, height);
    if (coords.width == 0 or coords.height == 0) {
        return;
    }

    // fill
    if (hasFill) {
        for (int row = coords.y; row < coords.y + coords.height; ++row) {
            for (int col = coords.x; col < coords.x + coords.width; ++col) {
                g.plotPixel(col, row, fillColor);
            }
        }
    }

    // top outline
    for (int row = coords.y;
         row < coords.y + outlineThickness - coords.topCutOff; ++row) {
        for (int col = coords.x; col < coords.x + coords.width; ++col) {
            g.plotPixel(col, row, outlineColor);
        }
    }
    // bottom outline
    for (int row =
            coords.y + coords.height - outlineThickness + coords.bottomCutOff;
         row < coords.y + coords.height;
         ++row) {
        for (int col = coords.x; col < coords.x + coords.width; ++col) {
            g.plotPixel(col, row, outlineColor);
        }
    }

    // left outline
    for (int col = coords.x;
         col < coords.x + outlineThickness - coords.leftCutOff; ++col) {
        for (int row = coords.y; row < coords.y + coords.height; ++row) {
            g.plotPixel(col, row, outlineColor);
        }
    }
    // right outline
    for (int col = coords.x + coords.width - outlineThickness + coords.rightCutOff;
         col < coords.x + coords.width;
         ++col)
    {
        for (int row = coords.y; row < coords.y + coords.height; ++row) {
            g.plotPixel(col, row, outlineColor);
        }
    }


}