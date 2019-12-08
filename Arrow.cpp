#include <cmath>
#include "Arrow.h"
#include "Rectangle.h"

void Arrow::draw(SDL_Plotter &g) {
    drawArrow(g, fromX, fromY, toX, toY, thickness, color);
}


void Arrow::drawArrow(SDL_Plotter &g, int fromX, int fromY, int toX, int toY, int thickness, Color c) {
	const double PI = acos(-1);
	const double ARROWHEAD_ANGLE = PI / 4;
	const int ARROW_LENGTH = 6;

	// Draw body of arrow
	drawLine(g, fromX, fromY, toX, toY, thickness, c);

	// Draw arrowhead
	double arrowAngle = atan2(toY - fromY, toX - fromX);
	drawLine(g, toX, toY, toX + (ARROW_LENGTH * cos(PI - ARROWHEAD_ANGLE + arrowAngle)), toY + (ARROW_LENGTH * sin(PI - ARROWHEAD_ANGLE + arrowAngle)), thickness, c);
	drawLine(g, toX, toY, toX + (ARROW_LENGTH * cos(ARROWHEAD_ANGLE - PI + arrowAngle)), toY + (ARROW_LENGTH * sin(ARROWHEAD_ANGLE - PI + arrowAngle)), thickness, c);
}

void Arrow::drawLine(SDL_Plotter &g, int fromX, int fromY, int toX, int toY, int thickness, Color c) {
	int xDiff = toX - fromX;
	int yDiff = toY - fromY;
	float length = sqrt(xDiff*xDiff + yDiff*yDiff);
	int numSamples = (int)(length);
	float curX = (float)fromX;
	float curY = (float)fromY;
	float dX = (float)(xDiff) / (float)(numSamples);
	float dY = (float)(yDiff) / (float)(numSamples);
	bool inBounds = (int)curX >= -thickness
		&& (int)curX + thickness < g.getCol()
		&& (int)curY >= -thickness
		&& (int)curY + thickness < g.getRow();
	for (int i = 0; i < numSamples; ++i) {
	    if (inBounds) {
            Rectangle rect{(int)curX, (int)curY, thickness, thickness, 0, Color::BLACK(), true, c};
            rect.draw(g);
	    }
		curX += dX;
		curY += dY;
		inBounds = (int)curX >= -thickness
		   && (int)curX + thickness < g.getCol()
		   && (int)curY >= -thickness
		   && (int)curY + thickness < g.getRow();
	}
}
