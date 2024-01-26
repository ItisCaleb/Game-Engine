#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL2/SDL.h>


class Sprite {
    public:
        Sprite(SDL_Texture *texture, int offX, int offY, int w, int h)
            :texture(texture),offX(offX),offY(offY),w(w),h(h){}

        void render(SDL_Renderer *renderer, float x, float y);
        int getWidth(){
            return w;
        }
        int getHeight(){
            return h;
        }
        SDL_Texture* getTexture(){
            return texture;
        }
    private:
        SDL_Texture *texture;
        int offX, offY;
        int w, h;

};


#endif