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
	Color color;
	int fullWidth;

public:
	CharacterGraphics(const string& filepath, const string& chars, Color col=Color::BLACK());

	void draw(SDL_Plotter &p, int x, int y, char c);
	void draw(SDL_Plotter &p, int x, int y, const string& s);

	void setColor(Color col);
	int getWidth() {return width;}
	int getHeight() {return height;}
	int getSpacing() {return 3;}
};

#endif /* CHARACTERGRAPHICS_H_ */
