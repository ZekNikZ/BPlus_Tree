#ifndef RENDERER_H
#define RENDERER_H

#include "BPlusTree.h"
#include "SDL_Plotter.h"
#include "BitmapImage.h"
#include <unordered_map>
#include <sstream>

class BPlusTreeRenderer {
public:
    BPlusTreeRenderer();

    template<typename T>
    void draw(SDL_Plotter & g, const BPlusTree<T>& tree, int x=0, int y=0);
private:
    template <class T>
    void drawNode(SDL_Plotter& g, const typename BPlusTree<T>::Node& node, int x = 0, int y = 0);

    unordered_map<char, BitmapImage> charImages;
};

// Templated functions have to go in same file

template<typename T>
void BPlusTreeRenderer::draw(SDL_Plotter &g, const BPlusTree<T> &tree, int x, int y) {

}

template<class T>
void BPlusTreeRenderer::drawNode(SDL_Plotter &g, const typename BPlusTree<T>::Node &node, int x, int y) {
    ostringstream ss;
    node >> ss;
    for (char c : ss.str()) {
        charImages[c].draw(g, 0, 0);
    }
}

#endif // RENDERER_H
