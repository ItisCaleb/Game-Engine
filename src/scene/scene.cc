#include "scene/scene.h"

#include "game/game.h"

void Scene::renderBackground(SDL_Renderer* renderer){
    if(!this->background) return;

    int bgWidth, bgHeight;
    Camera &camera = Game::getCamera();
    SDL_QueryTexture(this->background, nullptr, nullptr, &bgWidth, &bgHeight);
    // set source rect to cover the whole background
    SDL_Rect srcRect;
    srcRect.w = Game::getWidth();
    srcRect.h = Game::getHeight();
    srcRect.x = (int)camera.getX();
    srcRect.y = (int)camera.getY();

    float zoom = camera.getZoom();

    //check if source rect is out of bounds
    srcRect.x = std::max(0, std::min(srcRect.x, bgWidth - srcRect.w));
    srcRect.y = std::max(0, std::min(srcRect.y, bgHeight - srcRect.h));
    // set destination rect to cover the whole screen
    SDL_Rect destRect = {0, 0, srcRect.w * zoom, srcRect.h * zoom};
    // apply camera
    destRect = camera.apply( destRect);
    // render
    SDL_RenderCopy(renderer, this->background, &srcRect, &destRect);
}

Scene::Scene(int width, int height)
:width(width), height(height){}

int Scene::getWidth(){
    return this->width;
}

int Scene::getHeight(){
    return this->height;
}