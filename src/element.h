#ifndef ELEMENT_H
#define ELEMENT_H

#include <SDL2/SDL.h>

class Element {
public:
    Element(){}
    virtual void display() = 0;
    void move_left(int x){
        this->x -= x;
    }

    int x, y;
    int bump_width, bump_height;
};

#endif
