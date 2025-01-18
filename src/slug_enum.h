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
    JUMPING_OFF,

    // turning into another slug
    CHANGING,
};

class SlugStateRecord {
    SlugStateRecord(SlugState state){
        switch (state){
            case SlugState::MOVING:
                frameCount = 2;
                animationType = LOOP;
                slugTextureRow = 0;
                decorator = NONE;
                break;
            default:
                frameCount = 0;
                animationType = LOOP;
                slugTextureRow = 0;
                decorator = NONE;
                std::cout << "Invalid state" << std::endl;
                break;
        }
    }
    int frameCount;
    enum animationTypeEnum {ONCE, LOOP};
    animationTypeEnum animationType;
    int slugTextureRow;

    enum decoratorEnum {NONE, SPARKLE, BIG_SPARKLE, UPUP};
    decoratorEnum decorator;

    int frame;
};