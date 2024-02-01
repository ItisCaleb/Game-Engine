#ifndef WALL_H_
#define WALL_H_
#include <SDL2/SDL.h>
#include "engine/sprite.h"
#include "engine/object.h"
#include "engine/collide_shape.h"

class Wall: public Object{
    public:
        Wall(float x, float y, int width, int height, Sprite *sprite)
            :Object("Wall",x, y, 0), sprite(sprite),
            hitbox(x, y, width, height, this){
                Game::getScene()->addCollideShape(&hitbox);
            }
        void update(float dt){};
        void render(SDL_Renderer *renderer){
            if(sprite != NULL) sprite->render(renderer,this->x,this->y);
        }
        BoxCollideShape* getHitbox(){
            return &hitbox;
        }
    private:
        Sprite* sprite;
        BoxCollideShape hitbox;
};

#endif