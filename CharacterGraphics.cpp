/*
 * CharacterGraphics.cpp
 *
 *  Created on: Nov 30, 2019
 *      Author: matthewmccaskill
 */

#include "CharacterGraphics.h"

CharacterGraphics::CharacterGraphics(const string& filepath, const string& chars, Color col) : BitmapImage(filepath), color(col), chars(chars) {
	fullWidth = width;
	width /= chars.length();
}

void CharacterGraphics::draw(SDL_Plotter &p, int x, int y, char c) {
	int charIndex = chars.find(c);

    // TODO: Fix if part of it gets cut off at edge
    for (int row = 0; row < height; ++row) {
        for (int col = charIndex * width; col < charIndex * width + width; ++col) {
            Color& pixel = data[row * fullWidth + col];
            if (pixel.red == 0)
            	p.plotPixel(x + col - charIndex * width, y + row, color.red, color.green, color.blue);
        }
    }

}

void CharacterGraphics::draw(SDL_Plotter &p, int x, int y, const string& s) {
	const int SPACING = getSpacing();

	for (int i = 0; i < s.length(); ++i) {
		draw(p, x + (width + SPACING) * i, y, s[i]);
	}
}

void CharacterGraphics::setColor(Color col) { color = col;}
