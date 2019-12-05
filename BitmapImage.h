#ifndef PLOTTER_TEST_BITMAPIMAGE_H
#define PLOTTER_TEST_BITMAPIMAGE_H

#include <vector>
#include "SDL_Plotter.h"
#include "Color.h"

using namespace std;

class BitmapImage {
public:
    explicit BitmapImage(const string& filepath);
    explicit BitmapImage(const string& filepath, Color transparentColor);
    explicit BitmapImage(istream& in);
    explicit BitmapImage(istream& in, Color transparentColor);
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void draw(SDL_Plotter &p, int x, int y);

protected:
    int32_t width;
    int32_t height;
    vector<Color> data;
    bool load(istream& in, bool useTransparent, Color transparentColor = Color::WHITE());
};


#endif //PLOTTER_TEST_BITMAPIMAGE_H
