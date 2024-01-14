#ifndef PLAYER_H_
#define PLAYER_H_
#include "entity.h"

class Player : public Entity{
    public:
        Player();
        ~Player();
        void update(float dt);
        void render(SDL_Renderer *renderer);
        
};

#endif