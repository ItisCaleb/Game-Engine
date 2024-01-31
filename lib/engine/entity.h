#ifndef ENTITY_H_
#define ENTITY_H_
#include <SDL2/SDL.h>
#include <vector>
#include "engine/sprite.h"
#include "engine/object.h"

class Entity: public Object{
    public:
        Entity(std::string tag, float x, float y, int width, int height)
            :Object(tag, 0), x(x), y(y), width(width), height(height){};
        void chooseCurrentSprite(int i){
            if(i >= sprites.size()) return;
            this->currentSprite = i;
        }
        int getSpritesLength(){
            return sprites.size();
        }
        void setX(float x){
            this->x = x;
        }

        void setY(float y){
            this->y = y;
        }

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
        int currentSprite;
        std::vector<Sprite*> sprites;
        float x,y;
        int width, height;
};

#endif