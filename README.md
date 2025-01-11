# Slug Racing

A simple racing game built with C++ and SDL2.

## Prerequisites

- C++ compiler (g++ or MinGW for Windows)
- SDL2 library

## Installation

### Windows
1. Download SDL2 development libraries for MinGW from [SDL2 Releases](https://github.com/libsdl-org/SDL/releases/latest)
2. Extract `SDL2.dll` from `x86_64-w64-mingw32/bin/` to your project folder
3. Add SDL2's `include` and `lib` folders from `x86_64-w64-mingw32/` to your MinGW installation (typically `C:\MinGW\`)
   - Copy contents of `include/SDL2` to `C:\MinGW\include\SDL2`
   - Copy contents of `lib` to `C:\MinGW\lib`

## Building

### Windows

cmake -G "MinGW Makefiles" ..
mingw32-make

## Running
./slug_racing.exe