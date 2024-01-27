#ifndef ENTITY_H_
#define ENTITY_H_
#include <SDL2/SDL.h>
#include <vector>
#include "resource/sprite.h"
#include "object/object.h"

class Entity: public Object{
    public:
        Entity(float x, float y, int width, int height)
            :Object(ObjectType::Entity),
            x(x), y(y), width(width), height(height){};
        float getX(){
            return x;
        }
        float getY(){
            return y;
        }
        int getWidth(){
            return width;
        }
        int getHeight(){
            return height;
        }

    protected:
        std::vector<Sprite*> sprites;
        float x,y;
        int width, height;
};

#endif