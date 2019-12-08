cmake_minimum_required(VERSION 3.10)
project(BPlus_Tree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lmingw32")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

add_executable(BPlus_Tree main.cpp SDL_Plotter.h SDL_Plotter.cpp BitmapImage.cpp
        BitmapImage.h BPlusTree.h Arrow.h Rectangle.h BPlusTreeRenderer.h
        SDL_Plotter.h SDL_Plotter.cpp Color.h Rectangle.cpp
        ScreenArea.h Arrow.cpp CharacterGraphics.h CharacterGraphics.cpp
        SDL_Plotter.h SDL_Plotter.cpp ScreenArea.cpp)
target_link_libraries(BPlus_Tree SDL2main SDL2 SDL2_mixer)