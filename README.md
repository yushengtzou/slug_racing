# Slug Racing

A simple racing game built with C++ and SDL2.

## Prerequisites

- C++ compiler (g++ or MinGW for Windows)
- SDL2 library

## Installation

### Windows
1. Install MSYS2 from [MSYS2 Website](https://www.msys2.org/)
2. Open MSYS2 MINGW64 terminal and run:   ```bash
   pacman -Syu
   pacman -S --overwrite "*" mingw-w64-x86_64-SDL2   ```
3. Copy SDL2.dll to your project folder:   ```bash
   cp /mingw64/bin/SDL2.dll /path/to/your/project/   ```

## Building

### Windows
g++ -std=c++11 main.cpp -o slug_racing -I"C:\msys64\mingw64\include\SDL2" -L"C:\msys64\mingw64\lib" -lmingw32 -lSDL2main -lSDL2 -mwindows

### Linux

g++ -std=c++11 main.cpp -o lottery_game $(pkg-config --cflags --libs sdl2)

## Running
./lottery_game