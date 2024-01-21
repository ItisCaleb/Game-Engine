#include "camera.h"

//update camera position
void Camera::update(float targetX, float targetY) {
    x = targetX - (view_width / 2.0f);
    y = targetY - (view_height / 2.0f);
}
// screen = world - camera
SDL_Rect Camera::apply(SDL_Rect rect) {
    return SDL_Rect{.x = rect.x - (int)this->x, .y = rect.y - (int)this->y, .w = rect.w, .h = rect.h};
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
