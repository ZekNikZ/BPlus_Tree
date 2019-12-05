#ifndef BPLUS_TREE_COLOR_H
#define BPLUS_TREE_COLOR_H

#include <cstdint>

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    static Color BLACK() { return Color{0,0,0}; }
    static Color WHITE() { return Color{255,255,255}; }
    static Color RED() { return Color{255,0,0}; }
    static Color GREEN() { return Color{0,255,0}; }
    static Color BLUE() { return Color{0,0,255}; }
    static Color YELLOW() { return Color{255,255,0}; }
};

#endif //BPLUS_TREE_COLOR_H
