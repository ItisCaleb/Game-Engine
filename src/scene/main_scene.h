#ifndef MAIN_SCENE_H_
#define MAIN_SCENE_H_

#include <vector>

#include "scene/scene.h"
#include "resource/sprite.h"
#include "object/object.h"

class MainScene: public Scene{
    public:
        MainScene();
        ~MainScene();
        void update(float dt);
        void render(SDL_Renderer *renderer);
        void destroy();
    private:
        std::vector<Object*> objects;
};



#endif