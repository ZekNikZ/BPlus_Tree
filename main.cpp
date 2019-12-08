#include <iostream>
#include <chrono>
#include <string>
#include "SDL_Plotter.h"
#include "BPlusTree.h"
#include "BPlusTreeRenderer.h"
#include "BitmapImage.h"

using namespace std;

template <class T>
bool handleInput(BPlusTree<T>& tree, const string& input);

int main(int argc, char* argv[]) {
    static const int TARGET_FPS = 30;
    static const int TARGET_FRAME_MILLISECONDS = 1000 / TARGET_FPS;
    static const int TREE_MOVE_SPEED = 8;

    SDL_Plotter plotter(720, 960);
    BPlusTreeRenderer<int> renderer;

    BPlusTree<int> tree;
    int treeX = 0;
    int treeY = 0;

    string input;
    CharacterGraphics inputFont("images/ubuntu-mono-48pt.bmp", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    CharacterGraphics infoFont("images/ubuntu-mono-24pt.bmp", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

    BitmapImage cheesyChristmasBackground("images/christmasBg.bmp");

    bool quitCommand = false;
    while (!plotter.getQuit() && !quitCommand) {
        auto startTime = chrono::system_clock::now();
        // Poll for events
        if (plotter.kbhit()) {
            // Temporary fix for SDL_Plotter's janky event polling
            SDL_Event e;
            while (SDL_PollEvent(&e)) {}

            // Handle input
            char key = plotter.getKey();
            if (key == SDL_SCANCODE_RETURN) {
                quitCommand = handleInput(tree, input);
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
        inputFont.draw(plotter, 10, plotter.getRow() - 35 - inputFont.getHeight(), input);
        ostringstream info;
        info << "M " << tree.getM() << "   L " << tree.getL() << "         INSERT  REMOVE  MAKE EMPTY  SET M  SET L";
        infoFont.draw(plotter,10, plotter.getRow() - 30, info.str());
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
void setM(BPlusTree<T>& tree, const string& m) {
    int intM = parseArg<T>(m);
    if (intM > 2) {
        tree.setMAndClear(intM);
    }
}

template <class T>
void setL(BPlusTree<T>& tree, const string& l) {
    int intL = parseArg<T>(l);
    if (intL > 1) {
        tree.setLAndClear(intL);
    }
}

template <class T>
bool handleInput(BPlusTree<T>& tree, const string& input) {
    istringstream ss(input);
    string command;
    vector<string> args;
    ss >> command;
    string arg;
    while (ss >> arg) {
        args.push_back(arg);
    }

    if ((command == "INSERT" || command == "I") && args.size() == 1) {
        tree.insert(parseArg<T>(args[0]));
    }
    else if ((command == "REMOVE" || command == "R") && args.size() == 1) {
        tree.remove(parseArg<T>(args[0]));
    }
    else if ((command == "MAKE" && args.size() == 1 && args[0] == "EMPTY")
        || (command == "ME") || command == "CLEAR") {
        tree.makeEmpty();
    }
    else if (command == "SET" && args.size() == 2) {
        if (args[0] == "M") {
            setM(tree, args[1]);
        }
        else if (args[0] == "L") {
            setL(tree, args[1]);
        }
    }
    return (command == "QUIT" || command == "Q");
}
