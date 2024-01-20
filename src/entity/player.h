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
        float getX() const;
        float getY() const;

    private:
        BoxCollideShape hitbox;
        BoxCollideShape hurtbox;
        int velocity;
        int acceleration;
};

#endif