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

int main(int argc, char* argv[]) {
    static const int TARGET_FPS = 30;
    static const int TARGET_FRAME_MILLISECONDS = 1000 / TARGET_FPS;
    static const int TREE_MOVE_SPEED = 8;
    int treeX = 0;
    int treeY = 0;

    SDL_Plotter plotter(720, 960);
    BPlusTreeRenderer renderer;
    BPlusTree<int> tree = BPlusTree<int>::makeTestTree();


    while (!plotter.getQuit()) {
        auto startTime = chrono::system_clock::now();
        // Poll for events
        while (plotter.kbhit()) {

        }
        // Handle input
        switch (plotter.getKey()) {
            case UP_ARROW:
                treeY -= TREE_MOVE_SPEED; break;
            case DOWN_ARROW:
                treeY += TREE_MOVE_SPEED; break;
            case LEFT_ARROW:
                treeX -= TREE_MOVE_SPEED; break;
            case RIGHT_ARROW:
                treeX += TREE_MOVE_SPEED; break;
        }


        plotter.clear();
        renderer.draw(plotter, tree, treeX, treeY);
        plotter.update();

        auto timeDiff = chrono::system_clock::now() - startTime;
        int ms = chrono::duration_cast<chrono::milliseconds>(timeDiff).count();
        if (ms < TARGET_FRAME_MILLISECONDS) {
            plotter.Sleep(TARGET_FRAME_MILLISECONDS - ms);
        }
    }

    return 0;
}
