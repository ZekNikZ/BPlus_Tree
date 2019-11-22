#define SDL_MAIN_HANDLED
#include <iostream>
#include "SDL_Plotter.h"
#include "BitmapImage.h"
#include <chrono>

using namespace std;

int main() {
    SDL_Plotter plotter;
    BitmapImage testImg("images/coolImage.bmp");
    const int TARGET_FPS = 30;
    const int TARGET_FRAME_MILLISECONDS = 1000 / TARGET_FPS;

    while (!plotter.getQuit()) {
        auto startTime = chrono::system_clock::now();
        plotter.pollForEvents();
        plotter.clear();
        testImg.draw(plotter, 10, 10);
        plotter.update();
        auto timeDiff = chrono::system_clock::now() - startTime;
        int ms = chrono::duration_cast<chrono::milliseconds>(timeDiff).count();
        if (ms < TARGET_FRAME_MILLISECONDS) {
            plotter.Sleep(TARGET_FRAME_MILLISECONDS - ms);
        }
    }

    return 0;
}