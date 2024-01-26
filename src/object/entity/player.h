#ifndef PLAYER_H_
#define PLAYER_H_
#include "entity.h"
#include "misc/collide_shape.h"

class Player : public Entity{
    public:
        Player();
        ~Player();
        void update(float dt);
        void render(SDL_Renderer *renderer);

    private:
        BoxCollideShape hitbox;
        int speed;
        
};

#endif