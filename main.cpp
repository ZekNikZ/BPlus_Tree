#include <iostream>
#include "SDL_Plotter.h"

using namespace std;

int main() {
    SDL_Plotter plotter;

    while (!plotter.getQuit()) {
        plotter.kbhit();
    }

    return 0;
}