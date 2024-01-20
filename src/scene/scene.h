#ifndef SCENE_H_
#define SCENE_H_

#include <SDL2/SDL.h>

class Scene
{
protected:
    int width, height;
    SDL_Texture *background;
    SDL_Texture *foreground;
    void renderBackground(SDL_Renderer *renderer);
public:
    Scene(int width, int height);
    virtual void render(SDL_Renderer *renderer) = 0;

    //get this scene width and height.
    int getWidth();
    int getHeight();
};



#endif