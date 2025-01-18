#ifndef ELEMENT_H
#define ELEMENT_H

#include <SDL2/SDL.h>

class Element {
public:
    Element(){}
    virtual void display() = 0;
    void add_offset(int x, int y){
        this->x += x;
        this->y += y;
    }

    int x, y;
    int bump_width, bump_height;
};

#endif
