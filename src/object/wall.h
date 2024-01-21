#ifndef WALL_H_
#define WALL_H_
#include <SDL2/SDL.h>
#include "misc/sprite.h"
#include "object/object.h"
#include "misc/collide_shape.h"

class Wall: public Object{
    public:
        Wall(float x, float y, int width, int height, Sprite *sprite)
            :Object(ObjectType::Wall),
            x(x), y(y), width(width), height(height), sprite(sprite),
            hitbox(x, y, x+width, y+height){
                Game::addCollideShape(&hitbox, this);
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
        float x,y;
        int width, height;
};

#endif