#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

#define CAMERA_BORDER 1

class Camera {
   public:
    Camera(SDL_Renderer *renderer, int width, int height, int window_height);

    void update(float x, float y);
    void render();

   private:
    SDL_Renderer *renderer;
    SDL_Texture *target;

    int width;
    int height;
    int target_width;
    int target_height;
    int window_height;

    float x, y;
};

#endif  
