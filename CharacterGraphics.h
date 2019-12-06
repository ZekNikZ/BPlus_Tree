/*
 * CharacterGraphics.h
 *
 *  Created on: Nov 30, 2019
 *      Author: matthewmccaskill
 */

#ifndef CHARACTERGRAPHICS_H_
#define CHARACTERGRAPHICS_H_

#include <string>
#include <iostream>
#include "BitmapImage.h"

using namespace std;

class CharacterGraphics : public BitmapImage {
private:
	string chars;
	int charWidth;

public:
	CharacterGraphics(const string& filepath, const string& chars, Color transparentCol=Color::WHITE());

	void draw(SDL_Plotter &p, int x, int y, char c) const;
	void draw(SDL_Plotter &p, int x, int y, const string& s) const;

	void setColor(Color col);
	int getCharWidth() const { return charWidth; }
	int getSpacing() const {return 3;}
};

#endif /* CHARACTERGRAPHICS_H_ */
