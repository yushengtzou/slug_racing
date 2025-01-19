#pragma once
#include <iostream>

enum FoodType {
    RED,
    GREEN,
    BLUE,
    POOP
};

enum SlugType {
    AI_Benson,  // intern
    AI_Daniel,  // intern
    AI_Wes,     // employee
    DS,         // employee, 2 years
    MM_YuMing,  // employee
    MM_MuHuan,  // employee
    MM_Lulu     // employee, 3 years
};

enum SlugState {
    // default
    MOVING,

    // green: speed boosted
    BOOSTED,
    // red: speed boosted
    SUPER_BOOSTED,
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
    public:
        SlugStateRecord(SlugState state){
            this->frame = 0;
            this->animation_time = 0;
            this->state = state;
            switch (state){
                case SlugState::MOVING:
                    frameCount = 2;
                    animationType = LOOP;
                    slugTextureRow = 0;
                    animation_duration = -1;
                    break;
                case SlugState::SHIELDED:
                case SlugState::BOOSTED:
                case SlugState::SUPER_BOOSTED:
                    frameCount = 2;
                    animationType = LOOP;
                    slugTextureRow = 0;
                    animation_duration = 8;
                    break;
                case SlugState::STOPPED:
                    frameCount = 2;
                    animationType = LOOP;
                    slugTextureRow = 0;
                    animation_duration = 8;
                    break;
                default:
                    frameCount = 0;
                    animationType = LOOP;
                    slugTextureRow = 0;
                    animation_duration = -1;
                    break;
            }
        }
        // constants
        int frameCount;
        SlugState state;
        enum animationTypeEnum {ONCE, LOOP};
        animationTypeEnum animationType;
        int slugTextureRow;
        int animation_duration;

        // variables
        int frame;          // 0 to frameCount - 1
        int animation_time; // 0 to animationDuration - 1
};