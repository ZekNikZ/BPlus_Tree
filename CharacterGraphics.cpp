/*
 * CharacterGraphics.cpp
 *
 *  Created on: Nov 30, 2019
 *      Author: matthewmccaskill
 */

#include "CharacterGraphics.h"

CharacterGraphics::CharacterGraphics(const string& filepath, const string& chars, Color transparentCol) : BitmapImage(filepath, transparentCol), chars(chars) {
	charWidth = width / chars.length();
}

void CharacterGraphics::draw(SDL_Plotter &p, int x, int y, char c) const {
    if (c == ' ')
        return;
	int charIndex = chars.find(c);
    if (charIndex == -1) {
        cerr << "Couldn't draw letter \"" << c << "\" because it wasn't in the font\n";
        return;
    }
	drawPartial(p, x, y, charIndex*charWidth, 0, charWidth, height);
}

void CharacterGraphics::draw(SDL_Plotter &p, int x, int y, const string& s) const {
	const int SPACING = getSpacing();

	for (int i = 0; i < s.length(); ++i) {
		draw(p, x + (charWidth + SPACING) * i, y, s[i]);
	}
}

void CharacterGraphics::setColor(Color col) {
    for (Color& c : data) {
        if (!c.transparent)
            c = col;
    }
}
