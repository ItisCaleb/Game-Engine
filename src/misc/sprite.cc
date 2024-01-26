#include "misc/sprite.h"
#include "game/game.h"
Sprite::Sprite(SDL_Texture *texture, int offX, int offY, int w, int h)
:texture(texture),offX(offX),offY(offY),w(w),h(h){}

Sprite::~Sprite(){}

void Sprite::render(SDL_Renderer *renderer, float x, float y){
    Camera *camera = Game::getCamera();
    float zoom = camera->getZoom();
    SDL_FRect renderRect = {x, y, this->w, this->h};
    SDL_Rect clipRect = {this->offX, this->offY, this->w, this->h};
    // Apply camera
    renderRect = camera->apply(renderRect);
    SDL_RenderCopyF(renderer, this->texture, &clipRect, &renderRect);
}