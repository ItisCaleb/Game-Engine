#ifndef MAIN_SCENE_H_
#define MAIN_SCENE_H_

#include "scene/scene.h"
#include "misc/sprite.h"

class MainScene: public Scene{
    public:
        MainScene();
        ~MainScene();
        void render(SDL_Renderer *renderer);
};



#endif