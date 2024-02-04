#ifndef ENTITY_H_
#define ENTITY_H_
#include <SDL2/SDL.h>
#include <vector>
#include "engine/sprite.h"
#include "engine/object.h"

class Entity: public Object{
    public:
        Entity(std::string tag, float x, float y, int width, int height)
            :Object(tag, x, y, 0){};
        void chooseCurrentSprite(int i){
            if(i >= sprites.size()) return;
            this->currentSprite = i;
        }
        int getSpritesLength(){
            return sprites.size();
        }


    protected:
        int currentSprite;
        std::vector<Sprite*> sprites;
};

#endif