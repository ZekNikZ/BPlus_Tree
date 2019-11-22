#ifndef RENDERER_H
#define RENDERER_H

#include "BPlusTree.h"
#include "SDL_Plotter.h"
#include "BitmapImage.h"
#include <unordered_map>

class BPlusTreeRenderer {
private:
    template <class T>
    void drawNode(SDL_Plotter& g, const typename BPlusTree<T>::Node& node, int x = 0, int y = 0);
    unordered_map<char, BitmapImage> charImages;

public:
    BPlusTreeRenderer();
    template<typename T>
    void draw(SDL_Plotter & g, const BPlusTree<T>& tree, int x=0, int y=0);

};

#endif // RENDERER_H
