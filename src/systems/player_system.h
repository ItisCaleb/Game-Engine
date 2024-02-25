#ifndef PLAYER_SYSTEM_H
#define PLAYER_SYSTEM_H

#include "engine/ecs/system.h"

class PlayerSystem: public System{
    public:
        void init(){}
        void createPlayer();
        void update(float dt);
    private:
        Entity player;
};

#endif