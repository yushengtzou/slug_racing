#pragma once
#include <SDL2/SDL.h>
#include "slug_enum.h"

abstract class Element {
public:
    virtual Element(string type) = 0;
    virtual ~Element() = 0;
    virtual void display(int x, int y, int frame, string state) = 0;

    static int width, height;
    static int anchor_x, anchor_y;
};
