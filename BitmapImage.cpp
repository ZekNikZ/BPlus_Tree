#include <fstream>
#include <cassert>
#include "BitmapImage.h"
#include "ScreenArea.h"

BitmapImage::BitmapImage(const string &filepath) {
    width = height = 0;
    ifstream file(filepath, ios::in | ios::binary);
    if (!file) {
        cerr << "File not found: " << filepath << '\n';
        return;
    }
    load(file, false);
    file.close();
}

BitmapImage::BitmapImage(const string &filepath, Color transparentColor) {
    width = height = 0;
    ifstream file(filepath, ios::in | ios::binary);
    if (!file) {
        cerr << "File not found: " << filepath << '\n';
        return;
    }
    load(file, true, transparentColor);
    file.close();
}

BitmapImage::BitmapImage(istream &in) {
    width = height = 0;
    load(in, false);
}

BitmapImage::BitmapImage(istream &in, Color transparentColor) {
    width = height = 0;
    load(in, true, transparentColor);
}

template <typename T>
T read(istream& in) {
    T result;
    in.read((char*)&result, sizeof(result));
    return result;
}

bool BitmapImage::load(istream &in, bool useTransparent, Color transparentColor) {
    // see https://en.wikipedia.org/wiki/BMP_file_format#File_structure

    // Bitmap file header
    // 2 byte signature: "BM"
    // 4 bytes: File size
    // 4 bytes reserved space
    // 4 bytes: pixel data offset
    char header[2];
    in.read(header, 2);
    if (strncmp(header, "BM", 2) != 0) {
        cerr << "Couldn't load image file, BM header missing\n";
        return false;
    }
    // explicitly use uint32_t since the file format says it's a 4 byte filesize
    auto fileSize = read<uint32_t>(in);

    in.ignore(4); // skip reserved space

    auto pixelDataOffset = read<uint32_t>(in);

    // DIB header
    // This only supports the BITMAPINFOHEADER header, with this format:
    // 4 bytes: Size of header (40 in this version)
    // 4 bytes: Width of image (signed)
    // 4 bytes: Height of image (signed)
    // 2 bytes: # of color planes, must be 1
    // 2 bytes: # of bits per pixel, this impl. only supports 24
    // 4 bytes: Compression method, this impl. only supports 0 (None)
    // 4 bytes: image size
    // everything else is ignored in this implementation

    auto sizeOfDibHeader = read<uint32_t>(in);
    if (sizeOfDibHeader != 40) {
        cerr << "Couldn't read image file, DIB header not supported\n";
        return false;
    }

    // width and height are signed 32 bit ints
    int width = read<int32_t>(in);
    int height = read<int32_t>(in);

    auto numColorPlanes = read<uint16_t>(in);
    if (numColorPlanes != 1) {
        cerr << "Couldn't read image file, invalid number of color planes\n";
        return false;
    }

    auto bitsPerPixel = read<uint16_t>(in);
    if (bitsPerPixel != 24) {
        cerr << "Couldn't read image file, only 24bpp is supported\n";
        return false;
    }

    auto compressionMethod = read<uint32_t>(in);
    if (compressionMethod != 0) {
        cerr << "Couldn't read image file, compression not supported\n";
        return false;
    }

    auto imageSize = read<uint32_t>(in);
    // doesn't necessarily equal 3*width*height because of padding
    //if (3 * width * height != imageSize) {
    //    cerr << "Warning: imageSize = " << imageSize << ", 3*width*height = " << 3*width*height << "\n";
    //}

    data.resize(3*width*height);
    in.seekg(pixelDataOffset, ios::beg);
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            // rows are stored upside-down, so do height - 1 - r
            int pos = (height - 1 - r) * width + c;
            Color& color = data[pos];
            // pixels are stored out of order for some reason
            color.blue = read<unsigned char>(in);
            color.green = read<unsigned char>(in);
            color.red = read<unsigned char>(in);
            if (useTransparent && color == transparentColor)
            {
                color.transparent = true;
            }
        }
        // rows are padded to align every 4 bytes
        auto pos = in.tellg() - (streampos)pixelDataOffset;
        // if needed, go to next multiple of 4 by or-ing with 3 and adding 1
        int rem = (int)(pos % 4);
        if (rem) {
            pos += 4 - rem; // e.g. 9 has remainder of 1, so add 3 ---> 12
            in.seekg(pos + (streampos)pixelDataOffset, ios::beg);
        }
    }

    this->width = width;
    this->height = height;
    return true;
}

void BitmapImage::draw(SDL_Plotter &p, int x, int y) const {
    drawPartial(p, x, y, 0, 0, width, height);
}

void BitmapImage::drawPartial(SDL_Plotter &p, int x, int y, int colOffset, int rowOffset, int width, int height) const {
    assert(width <= this->width && height <= this->height);
    ScreenArea coords{p, x, y, width, height};
    // e.g: if 2 pixels got cut off at the top of the screen, start at row 2
    int imgRow = coords.topCutOff() + rowOffset;
    for (int yPos = coords.top(); yPos <= coords.bottom(); ++yPos) {
        int imgCol = coords.leftCutOff() + colOffset;
        for (int xPos = coords.left(); xPos <= coords.right(); ++xPos) {
            const Color& c = data[imgRow*this->width + imgCol];
            if (!c.transparent)
                p.plotPixel(xPos, yPos, c.red, c.green, c.blue);
            ++imgCol;
        }

        ++imgRow;
    }
}
