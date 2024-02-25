#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <SDL2/SDL.h>

#include "engine/ecs/system.h"
#include "engine/components/transform.h"
#include "engine/components/sprite.h"
#include "engine/game.h"

class RenderSystem: public System{
    public:
        void init(){
            auto ecs = Game::getECS();
            ComponentFlag flag;
            flag.set(ecs->queryType<Transform>());
            flag.set(ecs->queryType<Sprite>());
            ecs->setSystemFlag<RenderSystem>(flag);
        }

        void render(SDL_Renderer *renderer){
            Camera *camera = Game::getCamera();
            for (auto e: this->entities){
                auto &tf = Game::getECS()->query<Transform>(e);
                auto &sprite = Game::getECS()->query<Sprite>(e);
                SDL_FRect renderRect = {
                    tf.x, tf.y, 
                    sprite.w * tf.scaleX,
                    sprite.h * tf.scaleY};
                SDL_Rect clipRect = {
                    sprite.offX, sprite.offY,
                    sprite.w, sprite.h};
                // Apply camera
                renderRect = camera->apply(renderRect);
                SDL_RenderCopyExF(renderer, sprite.texture, &clipRect, 
                    &renderRect, 0, 0,
                    tf.flip?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);
            }
        }
};

#endif