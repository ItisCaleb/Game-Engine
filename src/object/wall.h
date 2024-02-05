#ifndef WALL_H_
#define WALL_H_
#include <SDL2/SDL.h>
#include "engine/sprite.h"
#include "engine/object.h"
#include "engine/collide_shape.h"

class Wall: public Object{
    public:
        Wall(float x, float y, int width, int height, Sprite *sprite)
            :Object("Wall",x, y, ObjectProperty::RIGID | ObjectProperty::STATIC), sprite(sprite){
                this->attachHitbox(new BoxCollideShape(width,height));
            }
        void update(float dt){};
        void render(SDL_Renderer *renderer){
            if(sprite != NULL) sprite->render(renderer,this->x,this->y);
        }
    private:
        Sprite* sprite;
};

#endif