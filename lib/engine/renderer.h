#ifndef RENDERER_H_
#define RENDERER_H_

#include <SDL2/SDL.h>

#include "engine/camera.h"

class Renderer{
    public:
        Renderer(SDL_Renderer *renderer, Camera *camera)
            :renderer(renderer), camera(camera){}   
        ~Renderer(){
            SDL_DestroyRenderer(this->renderer);
        }
        inline SDL_Renderer* getRenderer(){
            return this->renderer;
        }
        inline int SetRenderDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
            return SDL_SetRenderDrawColor(renderer, r, g, b ,a);
        }
        int renderCopyF(SDL_Texture *texture, SDL_Rect *srcrect, SDL_FRect *dstrect);
        int RenderDrawRectF(SDL_FRect *rect);
        int RenderDrawLineF(float x1, float y1, float x2, float y2);
        int RenderDrawPointF(float x, float y);

    private:
        SDL_Renderer *renderer;
        Camera *camera;
};


#endif