#define SDL_MAIN_HANDLED
#include <iostream>
#include "SDL_Plotter.h"
#include "BitmapImage.h"
#include "BPlusTree.h"
#include "BPlusTreeRenderer.h"
#include "Rectangle.h"
#include "Arrow.h"
#include "CharacterGraphics.h"
#include <chrono>

using namespace std;

int main() {
    const int TARGET_FPS = 30;
    const int TARGET_FRAME_MILLISECONDS = 1000 / TARGET_FPS;

    SDL_Plotter plotter;
    BPlusTreeRenderer renderer;
    Rectangle rect{10,10,100,50, 5, Color::BLACK(), true, Color::RED()};
    Arrow arr{10, 10, rect.getX(), rect.getY(), 4, Color::BLUE()};

    BPlusTree<int> tree = BPlusTree<int>::makeTestTree();

    while (!plotter.getQuit()) {
        auto startTime = chrono::system_clock::now();
        // Poll for events
        while (plotter.kbhit()) {

        }
        // Handle input
        switch (plotter.getKey()) {
            case UP_ARROW:
                rect.setY(rect.getY() - 4); break;
            case DOWN_ARROW:
                rect.setY(rect.getY() + 4); break;
            case LEFT_ARROW:
                rect.setX(rect.getX() - 4); break;
            case RIGHT_ARROW:
                rect.setX(rect.getX() + 4); break;
            case 'Q':
                rect.setWidth(rect.getWidth() + 5); break;
            case 'W':
                rect.setWidth(rect.getWidth() - 5); break;
        }

        arr.setToX(rect.getX());
        arr.setToY(rect.getY());

        plotter.clear();
        rect.draw(plotter);
        arr.draw(plotter);
        renderer.draw(plotter, tree, plotter.getCol() / 2, plotter.getRow() / 2);
        plotter.update();

        auto timeDiff = chrono::system_clock::now() - startTime;
        int ms = chrono::duration_cast<chrono::milliseconds>(timeDiff).count();
        if (ms < TARGET_FRAME_MILLISECONDS) {
            plotter.Sleep(TARGET_FRAME_MILLISECONDS - ms);
        }
    }

    return 0;
}
