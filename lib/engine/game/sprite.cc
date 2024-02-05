#include "engine/sprite.h"
#include "engine/game.h"

void Sprite::render(SDL_Renderer *renderer, float x, float y,
 float scaleX, float scaleY, bool flip){
    Camera *camera = Game::getCamera();
    float zoom = camera->getZoom();
    SDL_FRect renderRect = {x, y, this->w*scaleX, this->h*scaleY};
    SDL_Rect clipRect = {this->offX, this->offY, this->w, this->h};
    // Apply camera
    renderRect = camera->apply(renderRect);
    SDL_RenderCopyExF(renderer, this->texture, &clipRect, 
    &renderRect, 0, 0, 
    flip?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);
}