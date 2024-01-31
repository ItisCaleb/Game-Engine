#ifndef PLAYER_CONTROLLER_H_
#define PLAYER_CONTROLLER_H_
#include <SDL2/SDL.h>
#include <engine/resource_manager.h>
#include <engine/input_manager.h>
#include <engine/game.h>
#include "player.h"
class PlayerController{
    public:
        void hi(Player *u){
            if (InputManager::isKeyHold(InputManager::Key::S)){
                printf("i");
            }
        }
    private:
      
};

#endif