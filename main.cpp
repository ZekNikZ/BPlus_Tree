#include <iostream>
#include <chrono>
#include <string>
#include "SDL_Plotter.h"
#include "BPlusTree.h"
#include "BPlusTreeRenderer.h"
#include "BitmapImage.h"

using namespace std;

template <class T>
void handleInput(BPlusTree<T>& tree, const string& input);

int main(int argc, char* argv[]) {
    static const int TARGET_FPS = 60;
    static const int TARGET_FRAME_MILLISECONDS = 1000 / TARGET_FPS;
    static const int TREE_MOVE_SPEED = 8;

    SDL_Plotter plotter(720, 960);
    BPlusTreeRenderer renderer;

    BPlusTree<int> tree = BPlusTree<int>::makeTestTree();
    int treeX = 0;
    int treeY = 0;

    string input;
    CharacterGraphics inputFont("images/ubuntu-mono-48pt.bmp", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

    BitmapImage cheesyChristmasBackground("images/christmasBg.bmp");

    while (!plotter.getQuit()) {
        auto startTime = chrono::system_clock::now();
        // Poll for events
        if (plotter.kbhit()) {
            // Temporary fix for SDL_Plotter's janky event polling
            SDL_Event e;
            while (SDL_PollEvent(&e)) {}

            // Handle input
            char key = plotter.getKey();
            if (key == SDL_SCANCODE_RETURN) {
                handleInput(tree, input);
                input.clear();
            }
            else if (key == DOWN_ARROW) {
                if (!input.empty())
                    input.pop_back();
            }
            else if (isalnum(key) || key == ' ') {
                input += key;
            }
        }

        // Test for keyboard keys regardless of whether a keydown event was fired
        // so that it is smooth
        switch (plotter.getKey()) {
            case LEFT_ARROW:
                treeX -= TREE_MOVE_SPEED; break;
            case RIGHT_ARROW:
                treeX += TREE_MOVE_SPEED; break;
        }

        plotter.clear();
        cheesyChristmasBackground.draw(plotter, 0, 0);
        renderer.draw(plotter, tree, treeX, treeY);
        inputFont.draw(plotter, 10, plotter.getRow() - inputFont.getHeight(), input);
        plotter.update();

        auto timeDiff = chrono::system_clock::now() - startTime;
        int ms = chrono::duration_cast<chrono::milliseconds>(timeDiff).count();
        if (ms < TARGET_FRAME_MILLISECONDS) {
            plotter.Sleep(TARGET_FRAME_MILLISECONDS - ms);
        }
    }

    return 0;
}

template <class T>
T parseArg(const string& arg) {
    T result;
    istringstream ss(arg);
    ss >> result;
    return result;
}

template <class T>
void handleInput(BPlusTree<T>& tree, const string& input) {
    istringstream ss(input);
    string command, arg;
    ss >> command >> arg;

    if (command == "INSERT")
        tree.insert(parseArg<T>(arg));
    else if (command == "REMOVE")
        tree.remove(parseArg<T>(arg));
    else if (command == "MAKE" && arg == "EMPTY")
        tree.makeEmpty();
}