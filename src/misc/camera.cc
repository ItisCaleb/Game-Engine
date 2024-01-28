#include "misc/camera.h"
#include "game/game.h"

//update camera position
void Camera::update(float targetX, float targetY) {
    //zoom animation (smooth)    
    //printf("zoomStep: %f\n", this->zoomStep);
    if (zoom < targetZoom) {
        //printf("zoom: %f, targetZoom: %f\n", zoom, targetZoom);
        zoom += this->zoomStep;
        if (zoom > targetZoom) zoom = targetZoom;
    } else if (zoom > targetZoom) {
        //printf("zoom: %f, targetZoom: %f\n", zoom, targetZoom);
        zoom -= this->zoomStep;
        if (zoom < targetZoom) zoom = targetZoom;
    }

    //keep the target in the center of the screen
    x = targetX;
    y = targetY;

    //boundary check
    float newX = x;
    float newY = y;
    int width = Game::getWidth();
    int height = Game::getHeight();
    if (newX - width < -1280) {
        newX = width - 1280;
    } else if (newX + width > 3000) {
        newX = 3000 - width;
    }

    if (newY - height < -720) {
        newY = height - 720;;
    } else if (newY + height > 2000) {
        newY = 2000 - height;
    }
    //update position
    x=newX;
    y=newY;
}

// screen = world - camera
SDL_FRect Camera::apply(SDL_FRect &rect) {
    return SDL_FRect{
        .x = (((rect.x - this->x) * zoom) + Game::getWidth()/2) , 
        .y = (((rect.y - this->y) * zoom) + Game::getHeight()/2) , 
        .w = rect.w * zoom, 
        .h = rect.h * zoom};
}

void Camera::updateZoom(float targetZoom) {
    this->targetZoom = std::max(0.5f, std::min(targetZoom, 3.0f));
}

float Camera::applyX(float x){
    return x - this->x;
}
float Camera::applyY(float y){
    return y - this->y;
}

void Camera::setZoom(float zoomLevel) {
    zoom = zoomLevel > 0 ? zoomLevel : 1;
}
