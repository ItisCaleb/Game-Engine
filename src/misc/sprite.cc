#include "misc/sprite.h"
#include "game/game.h"
Sprite::Sprite(SDL_Texture *texture, int offX, int offY, int w, int h)
:texture(texture),offX(offX),offY(offY),w(w),h(h){}

Sprite::~Sprite(){}

void Sprite::render(SDL_Renderer *renderer, int x, int y){
    SDL_Rect renderRect = {x, y, this->w, this->h};
    SDL_Rect clipRect = {this->offX, this->offY, this->w, this->h};
    // Apply camera
    Camera &camera = Game::getCamera();
    renderRect = camera.apply(renderRect);
    SDL_RenderCopy(renderer, this->texture, &clipRect, &renderRect);
}

int Sprite::getWidth(){
    return this->w;
}

int Sprite::getHeight(){
    return this->h;
}