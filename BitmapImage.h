#ifndef PLOTTER_TEST_BITMAPIMAGE_H
#define PLOTTER_TEST_BITMAPIMAGE_H

#include <vector>
#include "SDL_Plotter.h"
#include "Color.h"

using namespace std;

/**
 * Represents an image using a grid of pixels.
 */
class BitmapImage {
public:
    /**
     * Create an image by loading the given .bmp file
     * @param filepath The .bmp to load
     */
    explicit BitmapImage(const string& filepath);

    /**
     * Create an image by loading the given .bmp, but with all pixels of the
     *   given color treated as transparent and skipped when drawn.
     * @param filepath the .bmp to load
     * @param transparentColor The color to treat as transparent
     */
    explicit BitmapImage(const string& filepath, Color transparentColor);

    /**
     * Create an image by reading data in the .bmp file format from the given
     *   stream.
     * @param in The stream to read data from.
     */
    explicit BitmapImage(istream& in);

    /**
     * Create an image by reading data in the .bmp file format from the given
     *   stream, but with all pixels of the given color treated as transparent
     *   and skipped when drawn.
     * @param in The stream to read data from.
     * @param transparentColor The color to treat as transparent.
     */
    explicit BitmapImage(istream& in, Color transparentColor);

    /**
     * Gets the width of the image in pixels.
     * @return The width.
     */
    int getWidth() const { return width; }
    /**
     * Gets the height of the image in pixels.
     * @return The height
     */
    int getHeight() const { return height; }

    /**
     * Draws the image to the given SDL_Plotter at the given location.
     * @param p The plotter to draw on.
     * @param x The x-coordinate of the top-left of the image.
     * @param y The y-coordinate of the top-right of the image.
     */
    void draw(SDL_Plotter &p, int x, int y) const;

    /**
     * Draws only a portion of the image to the given SDL_Plotter at the
     *   given location.
     * @param p The plotter to draw on.
     * @param x The x-coordinate of the top-left of the image.
     * @param y The y-coordinate of the top-right of the image.
     * @param colOffset The x-coordinate within the picture to start reading
     *   the pixels from.
     * @param rowOffset The y-coordinate within the picture to start reading
     *   the pixels from.
     * @param width The width to draw after colOffset.
     * @param height The height to draw after rowOffset.
     */
    void drawPartial(SDL_Plotter& p, int x, int y, int colOffset, int rowOffset, int width, int height) const;

protected:
    int32_t width;
    int32_t height;
    vector<Color> data;
    bool load(istream& in, bool useTransparent, Color transparentColor = Color::WHITE());
};


#endif //PLOTTER_TEST_BITMAPIMAGE_H
