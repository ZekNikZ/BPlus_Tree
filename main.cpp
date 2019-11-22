#define SDL_MAIN_HANDLED
#include <iostream>
#include "SDL_Plotter.h"
#include "BitmapImage.h"

using namespace std;

int main() {
    SDL_Plotter plotter;
    BitmapImage testImg("images/coolImage.bmp");

    while (!plotter.getQuit()) {
        plotter.kbhit();
        plotter.clear();
        testImg.draw(plotter, 10, 10);
        plotter.update();
    }

    return 0;
}