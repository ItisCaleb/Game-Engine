#ifndef WALL_H_
#define WALL_H_
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
        BoxCollideShape hurtbox;
        int speed;
};

#endif