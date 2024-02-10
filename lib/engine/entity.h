#ifndef ENTITY_H_
#define ENTITY_H_
#include <SDL2/SDL.h>
#include <vector>
#include "engine/sprite.h"
#include "engine/object.h"

class Entity: public Object{
    public:
        Entity(std::string tag, float x, float y)
            :Object(tag, x, y, 0){};
            
        void chooseCurrentSprite(size_t i){
            if(i >= sprites.size()) return;
            this->currentSprite = i;
        }

        std::vector<Sprite*>& getSprites(){
            return sprites;
        }
        
        int getSpritesLength(){
            return sprites.size();
        }


    protected:
        size_t currentSprite;
        std::vector<Sprite*> sprites;
};

#endif