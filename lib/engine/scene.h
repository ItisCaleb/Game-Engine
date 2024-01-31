#ifndef SCENE_H_
#define SCENE_H_

#include <SDL2/SDL.h>
#include <string>

#include "engine/sprite.h"

class Scene{
    public:
        Scene(int width, int height);
        virtual void update(float dt) = 0;
        virtual void render(SDL_Renderer *renderer) = 0;
        virtual void destroy() = 0;
    
        //get this scene width and height.
        int getWidth(){
            return width;
        }
        int getHeight(){
            return height;
        }
    protected:
        int width, height;
        Sprite *background;
        Sprite *foreground;
        void renderBackground(SDL_Renderer *renderer);
        void loadScene(std::string path);

};



#endif