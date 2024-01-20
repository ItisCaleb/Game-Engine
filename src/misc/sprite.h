#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL2/SDL.h>


class Sprite {
    private:
        SDL_Texture *texture;
        int offX, offY;
        int w, h;
    public:
        Sprite(SDL_Texture *texture, int offX, int offY, int w, int h);
        ~Sprite();
        void render(SDL_Renderer *renderer, int x, int y);
        int getWidth();
        int getHeight();
};


#endif