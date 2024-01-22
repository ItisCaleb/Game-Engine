#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>

class Camera {
public:
    // Default constructor
 Camera() : x(0), y(0), view_width(0), view_height(0), zoom(1.0f), zoomStep(0.05f) {}
 Camera(int width, int height) : x(0), y(0), view_width(width), view_height(height), zoom(1.0f), zoomStep(0.01f) {
    printf("Camera created\n");
    printf("zoomStep: %f\n", this->zoomStep);
 }

 void update(float targetX, float targetY);
 SDL_Rect apply(SDL_Rect &rect);
 void updateZoom(float targetZoom);
 int applyX(int x);
 int applyY(int y);
 void setZoom(float zoomLevel);
 float getZoom();
 float getX();
 float getY();
private:
    float x, y;
    int view_width, view_height;
    float zoom;
    float targetZoom=1.0f;
    float zoomStep=0.01f;
};

#endif