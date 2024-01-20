#ifndef BOX_H_
#define BOX_H_
#include "entity.h"
#include "misc/collide_shape.h"

class Box : public Entity{
    public:
        Box();
        ~Box();
        void update(float dt);
        void render(SDL_Renderer *renderer);
    private:
        BoxCollideShape hitbox;
        BoxCollideShape hurtbox;
        int speed;
};

#endif