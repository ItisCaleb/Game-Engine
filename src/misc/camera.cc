#include "camera.h"

void Camera::update(float targetX, float targetY) {
    x = targetX - (view_width / 2.0f ) ;
    y = targetY - (view_height / 2.0f );
    // x=targetX;
    // y=targetY;  
    printf("camera update: %f %f\n", x, y);
}

void Camera::apply(SDL_Renderer* renderer) {
    // SDL_Rect rect = getViewRect();
    // SDL_RenderSetViewport(renderer, &rect);
    int viewportX = static_cast<int>(x);
    int viewportY = static_cast<int>(y);
    SDL_Rect viewport = {viewportX, viewportY, static_cast<int>(view_width), static_cast<int>(view_height )};
    printf("apply: %d %d %d %d\n", viewportX, viewportY, static_cast<int>(view_width), static_cast<int>(view_height ));
    SDL_RenderSetViewport(renderer, &viewport);
}
SDL_Rect Camera::getCameraRect() const {
    // 计算相对于背景图像的源矩形
    int camX = static_cast<int>(x);
    int camY = static_cast<int>(y);
    int camW = static_cast<int>(view_width );
    int camH = static_cast<int>(view_height );
    return {camX, camY, camW, camH};
}
void Camera::setZoom(float zoomLevel) {
    zoom = zoomLevel > 0 ? zoomLevel : 1;
}

float Camera::getZoom(){
    return zoom;
}
float Camera::getX() {
    return x;
}
float Camera::getY() {
    return y;
}

SDL_Rect Camera::getViewRect() const {
    int viewportWidth = static_cast<int>(view_width / zoom);
    int viewportHeight = static_cast<int>(view_height / zoom);

    int viewportX = static_cast<int>(x - viewportWidth / 2);
    int viewportY = static_cast<int>(y - viewportHeight / 2);

    return {viewportX, viewportY, viewportWidth, viewportHeight};
    // int viewportX = static_cast<int>((x - view_width / 2.0f) * zoom);
    // int viewportY = static_cast<int>((y - view_height / 2.0f) * zoom);
    // int viewportWidth = static_cast<int>(view_width * zoom);
    // int viewportHeight = static_cast<int>(view_height * zoom);
    // printf("getViewRect:  %d %d %d %d\n", static_cast<int>(x * zoom), static_cast<int>(y * zoom), static_cast<int>(view_width * zoom), static_cast<int>(view_height * zoom));
    // return {viewportX, viewportY, viewportWidth, viewportHeight};
}
