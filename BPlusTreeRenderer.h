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
#include "CharacterGraphics.h"

class BPlusTreeRenderer {
public:
    BPlusTreeRenderer();

    template<typename T>
    void draw(SDL_Plotter & g, const BPlusTree<T>& tree, int x=0, int y=0);
private:

    CharacterGraphics characters;
    template<typename T>
    void drawNode(SDL_Plotter& g, const typename BPlusTree<T>::Node& node, int x = 0, int y = 0);

    unordered_map<char, BitmapImage> charImages;
};

//   explicit Rectangle(int x = 0, int y = 0, int width = 0, int height = 0, int outlineThickness = 1, Color outlineColor = Color::BLACK(), bool fill = false, Color fillColor = Color::BLACK())
template <typename T>
void BPlusTreeRenderer::drawNode(SDL_Plotter& g, const typename BPlusTree<T>::Node& node, int x, int y) {
    const int PADDING = 5;
    const int CHAR_WIDTH = characters.getWidth();
    const int CHAR_HEIGHT = characters.getHeight();
    const int CHAR_SPACING = characters.getSpacing();
    
    // Helper variables
    stringstream ss;
    string s;
    
    // Setup working rectangle
    Rectangle cell(0, 0, 0, 2 * PADDING + CHAR_HEIGHT, 1, node.type == BPlusTree<T>::Node::KEY ? Color::BLUE() : Color::GREEN(), true, node.type == BPlusTree<T>::Node::KEY ? Color::GREEN() : Color::YELLOW());
    cell.shiftX(x);
    cell.shiftY(y);
    Rectangle nodeRect = cell;
    nodeRect.setFill(false);
    nodeRect.setOutline(2);
    
    // Get data
    vector<T> data = node.vals;
    
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

        cell.shiftX(width - 1);
    }

    // Draw overall rectangle
    nodeRect.draw(g);
}


// Templated functions have to go in same file

template<typename T>
void BPlusTreeRenderer::draw(SDL_Plotter &g, const BPlusTree<T> &tree, int x, int y) {
    drawNode<T>(g, *(tree.root->ptrs[0]), x, y);
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
