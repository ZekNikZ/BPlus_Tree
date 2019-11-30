#ifndef RENDERER_H
#define RENDERER_H

#include "BPlusTree.h"
#include "SDL_Plotter.h"
#include "BitmapImage.h"
#include "Rectangle.h"
#include "Color.h"
#include <unordered_map>
#include <iostream>
#include <sstream>

class BPlusTreeRenderer {
public:
    BPlusTreeRenderer();

    template<typename T>
    void draw(SDL_Plotter & g, const BPlusTree<T>& tree, int x=0, int y=0);
private:
    template<typename T>
    void drawNode(SDL_Plotter& g, const typename BPlusTree<T>::Node& node, int x = 0, int y = 0);

    unordered_map<char, BitmapImage> charImages;
};

//   explicit Rectangle(int x = 0, int y = 0, int width = 0, int height = 0, int outlineThickness = 1, Color outlineColor = Color::BLACK(), bool fill = false, Color fillColor = Color::BLACK())
template <typename T>
void BPlusTreeRenderer::drawNode(SDL_Plotter& g, const typename BPlusTree<T>::Node& node, int x, int y) {
    const int PADDING = 5;
    const int CHAR_WIDTH = 5;
    const int CHAR_HEIGHT = 10;
    
    // Helper variables
    stringstream ss;
    string s;
    
    // Setup working rectangle
    Rectangle cell(0, 0, 0, 2 * PADDING + CHAR_HEIGHT, 1, node.type == BPlusTree<T>::Node::KEY ? Color::BLUE() : Color::GREEN(), true, node.type == BPlusTree<T>::Node::KEY ? Color::GREEN() : Color::BLUE());
    cell.shiftX(x);
    cell.shiftY(y);
    Rectangle nodeRect = cell;
    
    // Get data
    vector<T> data = node.vals;
    
    // Draw each value
    for (auto datum : data) {
        ss.clear();
        ss << datum;
        ss >> s;
        cout << s << ' ';
        int width = 2 * PADDING + CHAR_WIDTH * s.length();
        cell.setWidth(width);
        nodeRect.stretchX(width);
        cell.draw(g);
        cell.shiftX(width);
    }
    cout << "\nhere" << endl;
}


// Templated functions have to go in same file

template<typename T>
void BPlusTreeRenderer::draw(SDL_Plotter &g, const BPlusTree<T> &tree, int x, int y) {
    drawNode<T>(g, *(tree.root->ptrs[0]->ptrs[2]));
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
