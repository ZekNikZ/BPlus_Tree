cmake_minimum_required(VERSION 3.15)
project(BPlus_Tree)

set(CMAKE_CXX_STANDARD 11)

link_libraries(SDL2)
link_libraries(SDL2_mixer)

add_executable(BPlus_Tree main.cpp SDL_Plotter.h SDL_Plotter.cpp BitmapImage.cpp BitmapImage.h BPlusTree.h Line.h Rectangle.h BPlusTreeRenderer.h SDL_Plotter.h SDL_Plotter.cpp Color.h BPlusTreeRenderer.cpp Rectangle.cpp ScreenAdjustedCoords.h)