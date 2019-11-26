#define SDL_MAIN_HANDLED
#include <iostream>
#include "SDL_Plotter.h"
#include "BitmapImage.h"
#include "BPlusTree.h"
#include "BPlusTreeRenderer.h"
#include "Rectangle.h"
#include <chrono>

using namespace std;

int main() {
    SDL_Plotter plotter;
    BPlusTreeRenderer renderer;
    Rectangle rect{10,10,100,50, Color::BLACK(), 5, true, Color::RED()};
    const int TARGET_FPS = 30;
    const int TARGET_FRAME_MILLISECONDS = 1000 / TARGET_FPS;

    BPlusTree<int> tree = BPlusTree<int>::makeTestTree();

    while (!plotter.getQuit()) {
        auto startTime = chrono::system_clock::now();
        plotter.pollForEvents();

        if (plotter.isKeyDown(UP_ARROW))
            rect.setY(rect.getY() - 4);
        if (plotter.isKeyDown(DOWN_ARROW))
            rect.setY(rect.getY() + 4);
        if (plotter.isKeyDown(LEFT_ARROW))
            rect.setX(rect.getX() - 4);
        if (plotter.isKeyDown(RIGHT_ARROW))
            rect.setX(rect.getX() + 4);
        if (plotter.isKeyDown('Q'))
            rect.setWidth(rect.getWidth() + 5);
        if (plotter.isKeyDown('W'))
            rect.setWidth(rect.getWidth() - 5);

        plotter.clear();
        rect.draw(plotter);
        renderer.draw(plotter, tree, 20, 20);
        plotter.update();

        auto timeDiff = chrono::system_clock::now() - startTime;
        int ms = chrono::duration_cast<chrono::milliseconds>(timeDiff).count();
        if (ms < TARGET_FRAME_MILLISECONDS) {
            plotter.Sleep(TARGET_FRAME_MILLISECONDS - ms);
        }
    }

    return 0;
}