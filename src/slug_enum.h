#pragma once
#include <iostream>

enum SlugType {
    AI_Daniel,  // intern
    AI_Benson,  // intern
    AI_Wes,     // employee
    DS,         // employee, 2 years
    MM_Lulu,    // employee, 3 years
    MM_MuHuan,  // employee
    MM_YuMin    // employee
};

enum SlugState {
    // default
    MOVING,

    // speed boosted
    BOOSTED,
    // blue star: no harm can be done and speed is boosted
    SHIELDED,
    // poop: stopped
    STOPPED,

    // chased by a starfish
    CHASED,
    // being eaten by a starfish
    EATEN,
    // dead
    DEAD,

    // riding a starfish
    RIDING,
    JUMPING_OFF

    // turning into another slug
    CHANGING,
};

class SlugStateRecord {
    SlugStateRecord(SlugStateEnum state){
        switch (state){
            case SlugStateEnum::MOVING:
                frameCount = 2;
                animationType = LOOP;
                slugTextureRow = 0;
                break;
            default:
                std::cout << "Invalid state" << std::endl;
                break;
        }
    }
    const int frameCount;
    const enum animationType {ONCE, LOOP};
    const int slugTextureRow;

    const enum decorator {NONE, SPARKLE, BIG_SPARKLE, UPUP}

    int frame;
}