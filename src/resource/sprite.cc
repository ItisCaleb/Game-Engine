#include "resource/sprite.h"
#include "game/game.h"

void Sprite::render(SDL_Renderer *renderer, float x, float y,
 float scaleX, float scaleY){
    Camera *camera = Game::getCamera();
    float zoom = camera->getZoom();
    SDL_FRect renderRect = {x, y, this->w*scaleX, this->h*scaleY};
    SDL_Rect clipRect = {this->offX, this->offY, this->w, this->h};
    // Apply camera
    renderRect = camera->apply(renderRect);
    SDL_RenderCopyF(renderer, this->texture, &clipRect, &renderRect);
}