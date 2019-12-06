#include "Rectangle.h"
#include "ScreenArea.h"


void Rectangle::draw(SDL_Plotter &g) {
    ScreenArea coords(g, x, y, width, height);
    if (coords.width() == 0 or coords.height() == 0) {
        return;
    }

    // fill
    if (hasFill) {
        for (int row = coords.top(); row <= coords.bottom(); ++row) {
            for (int col = coords.left(); col <= coords.right(); ++col) {
                g.plotPixel(col, row, fillColor.red, fillColor.green, fillColor.blue);
            }
        }
    }

    if (outlineThickness > 0) {
        // top outline
        coords = ScreenArea(g, x, y, width, outlineThickness);
        for (int row = coords.top(); row <= coords.bottom(); ++row) {
            for (int col = coords.left(); col <= coords.right(); ++col) {
                g.plotPixel(col, row, outlineColor.red, outlineColor.green, outlineColor.blue);
            }
        }

        // bottom outline
        coords = ScreenArea(g, x, y + height - outlineThickness, width,
                           outlineThickness);
        for (int row = coords.top(); row <= coords.bottom(); ++row) {
            for (int col = coords.left(); col <= coords.right(); ++col) {
                g.plotPixel(col, row, outlineColor.red, outlineColor.green, outlineColor.blue);
            }
        }

        // left outline
        coords = ScreenArea(g, x, y, outlineThickness, height);
        for (int row = coords.top(); row <= coords.bottom(); ++row) {
            for (int col = coords.left(); col <= coords.right(); ++col) {
                g.plotPixel(col, row, outlineColor.red, outlineColor.green, outlineColor.blue);
            }
        }

        // right outline
        coords = ScreenArea(g, x + width - outlineThickness, y,
                           outlineThickness, height);
        for (int row = coords.top(); row <= coords.bottom(); ++row) {
            for (int col = coords.left(); col <= coords.right(); ++col) {
                g.plotPixel(col, row, outlineColor.red, outlineColor.green, outlineColor.blue);
            }
        }
    }
}
