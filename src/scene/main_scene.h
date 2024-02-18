#ifndef MAIN_SCENE_H_
#define MAIN_SCENE_H_

#include <vector>

#include <engine/scene.h>
#include <engine/sprite.h>
#include <engine/object.h>

#include "object/entity/player.h"

class MainScene: public Scene{
    public:
        MainScene();
        ~MainScene();
        void init();
        void update(float dt);
        void render(Renderer *renderer);
        void destroy();
};



#endif