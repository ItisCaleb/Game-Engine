#include "camera.h"

//update camera position
void Camera::update(float targetX, float targetY) {
    //zoom animation (smooth)
    if (zoom < targetZoom) {
        printf("zoom: %f, targetZoom: %f\n", zoom, targetZoom);
        zoom += zoomStep;
        if (zoom > targetZoom) zoom = targetZoom;
    } else if (zoom > targetZoom) {
        printf("zoom: %f, targetZoom: %f\n", zoom, targetZoom);
        zoom -= zoomStep;
        if (zoom < targetZoom) zoom = targetZoom;
    }

    //keep the target in the center of the screen
    x = targetX - (view_width / 2.0f);
    y = targetY - (view_height / 2.0f);

    //boundary check
    float newX = x;
    float newY = y;
    if (newX < -1280) {
        newX = x;
    } else if (newX + view_width > 3000) {
        newX = 3000 - view_width;
    }

    if (newY < -720) {
        newY = y;
    } else if (newY + view_height > 2000) {
        newY = 2000 - view_height;
    }
    //update position
    x=newX;
    y=newY;
}

// screen = world - camera
SDL_Rect Camera::apply(SDL_Rect &rect) {
    return SDL_Rect{
        .x = (int)((rect.x - (int)this->x) * zoom) , 
        .y = (int)((rect.y - (int)this->y) * zoom), 
        .w = (int)(rect.w * zoom), 
        .h = (int)(rect.h * zoom)};
}

void Camera::updateZoom(float targetZoom) {
    this->targetZoom = std::max(0.5f, std::min(targetZoom, 3.0f));
}

int Camera::applyX(int x){
    return x - this->x;
}
int Camera::applyY(int y){
    return y - this->y;
}

void Camera::setZoom(float zoomLevel) {
    zoom = zoomLevel > 0 ? zoomLevel : 1;
}


float Camera::getZoom() {
    return zoom;
}
float Camera::getX() {
    return x;
}
float Camera::getY() {
    return y;
}
