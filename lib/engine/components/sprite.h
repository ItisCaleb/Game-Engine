#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <SDL2/SDL.h>
#include "engine/ecs/ecs.h"

class Sprite: public Component {
    public:
        Sprite(){}
        Sprite(SDL_Texture *texture, int offX, int offY, int w, int h)
            :texture(texture),offX(offX),offY(offY),w(w),h(h){}

        Sprite(std::string path);

        SDL_Texture *texture;
        int offX, offY;
        int w, h;

};

#endif
