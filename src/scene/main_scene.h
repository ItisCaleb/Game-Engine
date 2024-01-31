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
        void update(float dt);
        void render(SDL_Renderer *renderer);
        void destroy();
    private:
        std::vector<Object*> objects;
        Player *player;
};



#endif