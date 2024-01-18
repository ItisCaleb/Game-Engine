#include "Camera.h"

Camera::Camera(SDL_Renderer *renderer, int width, int height, int window_height) : renderer(renderer), width(width), height(height), window_height(window_height), x(0), y(0) {
    target_width = width + CAMERA_BORDER * 2;
    target_height = height + CAMERA_BORDER * 2;
    target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, target_width, target_height);
    SDL_assert(target);
    SDL_SetTextureBlendMode(target, SDL_BLENDMODE_BLEND);
}

void Camera::update(float newX, float newY) {
    x = newX;
    y = newY;
}

void Camera::render() {
    // Camera target
    SDL_SetRenderTarget(renderer, target);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);

    // Draw Sprite logic should go here...

    // Screen target
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    // Draw camera texture
    float pixel_h = (float)window_height / height;  // Window_Height should be defined elsewhere or passed to the method
    float correction_x = (int)x - x;
    float correction_y = (int)y - y;

    SDL_Rect dst;
    dst.x = correction_x * pixel_h - pixel_h * CAMERA_BORDER;
    dst.y = correction_y * pixel_h - pixel_h * CAMERA_BORDER;
    dst.w = target_width * pixel_h;
    dst.h = target_height * pixel_h;

    SDL_RenderCopy(renderer, target, NULL, &dst);
}
