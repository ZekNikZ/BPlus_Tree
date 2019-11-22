#ifndef RENDERER_H
#define RENDERER_H

#include "BPlusTree.h"
#include "SDL_Plotter.h"
#include <unordered_map>

class Image {};

class BPlusTreeRenderer {
private:
    void drawNode(SDL_Plotter& g, const BPlusTree::Node& node, int x = 0, int y = 0);
    unordered_map<char, Image>;

public:
    BPlusTreeRenderer();
    template<typename T>
    void draw(SDL_Plotter & g, const BPlusTree<T>& tree, int x=0, int y=0);

};

#endif // RENDERER_H
