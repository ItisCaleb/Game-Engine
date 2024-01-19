#include "misc/sprite.h"


Sprite::Sprite(SDL_Texture *texture, int offX, int offY, int w, int h)
:texture(texture),offX(offX),offY(offY),w(w),h(h){}

Sprite::~Sprite(){}

void Sprite::render(SDL_Renderer *renderer, int x, int y){
    SDL_Rect renderRect = {x, y, this->w, this->h};
    SDL_Rect clipRect = {this->offX, this->offY,
                         this->w, this->h};
    Camera& cam = Game::getCamera();
    renderRect = cam.apply(renderer, renderRect);
    printf("renderRect: %d %d %d %d\n", renderRect.x, renderRect.y, renderRect.w, renderRect.h);
    SDL_RenderCopy(renderer, this->texture, &clipRect, &renderRect);
}