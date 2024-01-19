#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>

class Camera {
   public:
    // Default constructor
    Camera() : x(0), y(0), view_width(0), view_height(0), zoom(1.0f) {}
    Camera(int width, int height, float initialZoom) : x(0), y(0), view_width(width), view_height(height), zoom(initialZoom) {}

    void update(float targetX, float targetY);
    SDL_Rect apply(SDL_Renderer* renderer, SDL_Rect rect);
    // SDL_Rect applyRect;
    SDL_Rect getCameraRect() const;
    void setZoom(float zoomLevel);
    float getZoom();
    float getX();
    float getY();
//    private:
    float x, y;
    int view_width, view_height;
    float zoom;

    SDL_Rect getViewRect() const;
};

#endif
