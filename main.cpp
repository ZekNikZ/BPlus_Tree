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
    BPlusTree<int> tree = BPlusTree<int>::makeTestTree();

    int x = 10;
    int y = 10;
    BitmapImage testImg("images/coolImage.bmp", Color{239, 228, 176});

    while (!plotter.getQuit()) {
        auto startTime = chrono::system_clock::now();
        // Poll for events
        while (plotter.kbhit()) {

        }
        // Handle input
        switch (plotter.getKey()) {
            case UP_ARROW:
                y -= 4; break;
            case DOWN_ARROW:
                y += 4; break;
            case LEFT_ARROW:
                x -= 4; break;
            case RIGHT_ARROW:
                x += 4; break;
        }


        plotter.clear();
        testImg.draw(plotter, x, y);
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
