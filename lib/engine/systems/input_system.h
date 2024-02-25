#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "engine/ecs/system.h"
#include "engine/components/input.h"
#include "engine/game.h"

class InputSystem: public System{
    public:
        void init(){
            auto ecs = Game::getECS();
            ComponentFlag flag;
            flag.set(ecs->queryType<Input>());
            ecs->setSystemFlag<InputSystem>(flag);
        }

        void update(float dt){
            const Uint8* state = SDL_GetKeyboardState(NULL);
            // reset the key state
            for (auto e: this->entities){
                auto &input = Game::getECS()->query<Input>(e);
                input.prevKeyState = input.keyState;
                input.keyState = 0;
                // reset the mouse wheel scroll
                if (state[SDL_SCANCODE_W]) input.keyState |= Input::Key::W;
                if (state[SDL_SCANCODE_A]) input.keyState |= Input::Key::A;
                if (state[SDL_SCANCODE_S]) input.keyState |= Input::Key::S;
                if (state[SDL_SCANCODE_D]) input.keyState |= Input::Key::D;
                if (state[SDL_SCANCODE_J]) input.keyState |= Input::Key::J;
                if (state[SDL_SCANCODE_SPACE]) input.keyState |= Input::Key::SPACE;
                if (state[SDL_SCANCODE_ESCAPE]) input.keyState |= Input::Key::ESC;
            }
        }
};

#endif