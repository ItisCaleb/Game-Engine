#include "engine/renderer.h"

int Renderer::renderCopyF(SDL_Texture *texture, SDL_Rect *srcrect, SDL_FRect *dstrect){
    auto destRect = camera->apply(dstrect);
    return SDL_RenderCopyF(renderer, texture, srcrect, &destRect);
}

int Renderer::RenderDrawRectF(SDL_FRect *rect){
    auto destRect = camera->apply(rect);
    return SDL_RenderDrawRectF(renderer, &destRect);
}

int Renderer::RenderDrawLineF(float x1, float y1, float x2, float y2){
    float ax1 = camera->applyX(x1);
    float ay1 = camera->applyY(y1);
    float ax2 = camera->applyX(x2);
    float ay2 = camera->applyY(y2);
    return SDL_RenderDrawLineF(renderer, ax1, ay1, ax2 ,ay2);
}

int Renderer::RenderDrawPointF(float x, float y){
    return SDL_RenderDrawPoint(renderer, x, y);
}
