#ifndef RENDERER_H
#define RENDERER_H

#include <unordered_map>
#include <iostream>
#include <sstream>
#include "BPlusTree.h"
#include "SDL_Plotter.h"
#include "BitmapImage.h"
#include "Rectangle.h"
#include "Color.h"
#include "Arrow.h"
#include "CharacterGraphics.h"

class BPlusTreeRenderer {
public:
    BPlusTreeRenderer();

    template<typename T>
    void draw(SDL_Plotter & g, const BPlusTree<T>& tree, int x=0, int y=0);
private:
    template<typename T>
    struct RendNode {
    	RendNode(typename BPlusTree<T>::Node * node, int width=0, int x=-1) : node(node), width(width), x(x) {}
    	typename BPlusTree<T>::Node * node;
    	int x;
    	int width;
    	vector<RendNode *> children;
    	vector<int> valXs;
    };

    CharacterGraphics characters;
    template<typename T>
    void drawNode(SDL_Plotter& g, RendNode<T> * rendNode, int x = 0, int y = 0);

    template<typename T>
    int getRenderedNodeWidth(typename BPlusTree<T>::Node * node);

    unordered_map<char, BitmapImage> charImages;
};

//   explicit Rectangle(int x = 0, int y = 0, int width = 0, int height = 0, int outlineThickness = 1, Color outlineColor = Color::BLACK(), bool fill = false, Color fillColor = Color::BLACK())
static const int PADDING = 8;
template <typename T>
void BPlusTreeRenderer::drawNode(SDL_Plotter& g, RendNode<T> * rendNode, int x, int y) {
    static const int CHAR_WIDTH = characters.getCharWidth();
    static const int CHAR_HEIGHT = characters.getHeight();
    static const int CHAR_SPACING = characters.getSpacing();

    // Colors
    static const Color DATA_OUTLINE(82, 169, 250);
    static const Color DATA_BACKGROUND(173, 216, 255);
    static const Color KEY_OUTLINE(68, 227, 142);
    static const Color KEY_BACKGROUND(173, 255, 187);

    // Helper variables
    stringstream ss;
    string s;

    // Setup working rectangle
    Rectangle cell(0, 0, 0, 2 * PADDING + CHAR_HEIGHT, 1,
    		rendNode->node->type == BPlusTree<T>::Node::KEY ? KEY_OUTLINE : DATA_OUTLINE, true,
    				rendNode->node->type == BPlusTree<T>::Node::KEY ? KEY_BACKGROUND : DATA_BACKGROUND);
    cell.shiftX(x);
    cell.shiftY(y);
    Rectangle nodeRect = cell;
    nodeRect.setFill(false);
    nodeRect.setOutline(2);

    // Get data
    vector<T> data = rendNode->node->vals;

    // Draw each value
    for (auto datum : data) {
        ss.clear();
        ss << datum;
        ss >> s;
        int width = 2 * PADDING + CHAR_WIDTH * s.length() + CHAR_SPACING * (s.length() - 1);
        cell.setWidth(width);
        nodeRect.stretchX(width - 1);
        cell.draw(g);
        characters.draw(g, cell.getX() + PADDING, cell.getY() + PADDING, s);

        rendNode->valXs.push_back(cell.getX());
        cell.shiftX(width - 1);
    }
    rendNode->valXs.push_back(cell.getX());

    // Draw overall rectangle
    nodeRect.draw(g);
}

template <typename T>
int BPlusTreeRenderer::getRenderedNodeWidth(typename BPlusTree<T>::Node * node) {
    static const int CHAR_WIDTH = characters.getCharWidth();
    static const int CHAR_SPACING = characters.getSpacing();

    // Helper variables
    stringstream ss;
    string s;

    int fullWidth = 0;

    // Get data
    vector<T> data = node->vals;

    // Draw each value
    for (auto datum : data) {
        ss.clear();
        ss << datum;
        ss >> s;
        fullWidth += 2 * PADDING + CHAR_WIDTH * s.length() + CHAR_SPACING * (s.length() - 1);
    }

    return fullWidth;
}


// Templated functions have to go in same file

template<typename T>
void BPlusTreeRenderer::draw(SDL_Plotter &g, const BPlusTree<T> &tree, int x, int y) {
	static const int MIN_SPACE_BETWEEN_NODES = 15;
	static const int LEVEL_SEPARATION = 20;
	static const int CHAR_HEIGHT = characters.getHeight();
	static const Color ARROW_COLOR(120, 120, 120);
    
    if (tree.size == 0) return;

	if (tree.size == 0) return;

	vector<vector<RendNode<T> *>> levels;

	levels.push_back(vector<RendNode<T> *>());
	levels.front().push_back(new RendNode<T>(tree.root, getRenderedNodeWidth<T>(tree.root)));

	// Make level tree
	while (levels.back().front()->node->type != BPlusTree<T>::Node::DATA) {
		// Make next level
		levels.push_back(vector<RendNode<T> *>());

		// Add to next level
		for (auto & rendNode : levels[levels.size() - 2]) {
			for (auto child : rendNode->node->ptrs) {
				levels.back().push_back(new RendNode<T>(child, getRenderedNodeWidth<T>(child)));
				rendNode->children.push_back(levels.back().back());
			}
		}
	}

	// Compute width of bottom level
	int bottomLevelWidth = (levels.back().size() - 1) * MIN_SPACE_BETWEEN_NODES;
	for (auto rendNode : levels.back()) {
		bottomLevelWidth += rendNode->width;
	}

	// Compute total tree height
	int rowHeight = (2 * PADDING + CHAR_HEIGHT);
	int totalTreeHeight = rowHeight * levels.size() + LEVEL_SEPARATION * (levels.size() - 1);

	// Compute starting x and y location for drawing
	int xDraw = (g.getCol() - bottomLevelWidth) / 2 + x;
	int yDraw = (g.getRow() - totalTreeHeight) / 2 + totalTreeHeight - rowHeight + y;

	// Draw bottom row
	for (auto it = levels.back().begin(); it != levels.back().end(); ++it) {
		drawNode<T>(g, *it, xDraw, yDraw);
		(*it)->x = xDraw;
		if (it + 1 != levels.back().end()) {
			xDraw += (*it)->width + MIN_SPACE_BETWEEN_NODES;
			Arrow::drawArrow(g, xDraw - MIN_SPACE_BETWEEN_NODES, yDraw + rowHeight / 2, xDraw, yDraw + rowHeight / 2);
		}
	}

	// Draw other rows
	auto it = levels.rbegin();
	for (++it; it != levels.rend(); ++it) {
		yDraw -= LEVEL_SEPARATION + rowHeight;

		// Draw each node on this level
		for (auto & rendNode : *it) {
			xDraw = (rendNode->children.front()->x + rendNode->children.back()->x + rendNode->children.back()->width) / 2 - rendNode->width / 2;
			drawNode<T>(g, rendNode, xDraw, yDraw);
			rendNode->x = xDraw;

			// Draw arrows to children
			for (int i = 0; i < rendNode->children.size(); ++i) {
				Arrow::drawArrow(g, rendNode->valXs[i], yDraw + rowHeight, rendNode->children[i]->x, yDraw + LEVEL_SEPARATION + rowHeight);
			}
		}
	}

	// Clean up memory
	for (auto level : levels) {
		for (auto rendNode : level) {
			delete rendNode;
		}
	}
}
//
//template<class T>
//void BPlusTreeRenderer::drawNode(SDL_Plotter &g, const typename BPlusTree<T>::Node &node, int x, int y) {
//    ostringstream ss;
//    node >> ss;
//    for (char c : ss.str()) {
//        charImages[c].draw(g, 0, 0);
//    }
//}

#endif // RENDERER_H
