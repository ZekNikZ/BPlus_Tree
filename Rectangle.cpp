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
    ScreenAdjustedCoords coords2(g, x, y, width, outlineThickness);
    for (int row = coords2.y; row < coords2.y + coords2.height; ++row) {
        for (int col = coords2.x; col < coords2.x + coords2.width; ++col) {
            g.plotPixel(col, row, outlineColor);
        }
    }
    // bottom outline
    ScreenAdjustedCoords coords3(g, x, y + height - outlineThickness, width, outlineThickness);
    for (int row = coords3.y; row < coords3.y + coords3.height; ++row) {
        for (int col = coords3.x; col < coords3.x + coords3.width; ++col) {
            g.plotPixel(col, row, outlineColor);
        }
    }

    // left outline
    ScreenAdjustedCoords coords4(g, x, y, outlineThickness, height);
    for (int row = coords4.y; row < coords4.y + coords4.height; ++row) {
        for (int col = coords4.x; col < coords4.x + coords4.width; ++col) {
            g.plotPixel(col, row, outlineColor);
        }
    }

    // right outline
    ScreenAdjustedCoords coords5(g, x + width - outlineThickness, y, outlineThickness, height);
    for (int row = coords5.y; row < coords5.y + coords5.height; ++row) {
        for (int col = coords5.x; col < coords5.x + coords5.width; ++col) {
            g.plotPixel(col, row, outlineColor);
        }
    }
}