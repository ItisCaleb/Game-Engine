#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include <cstdint>
#include "engine/ecs/ecs.h"
class InputSystem;
class Input: public Component {
    public:
        friend InputSystem;
        enum Key {
            W = 1,  
            A = 2,  
            S = 4,  
            D = 8,  
            SPACE = 16,
            ESC = 32,
            J = 64,
            WASD = 15
        };
        //update the key state
        bool isKeyHold(Key key){
            return keyState & key;
        }
        bool isKeyRelease(Key key){
            // prev->1 now->0
            return !(keyState & key) && (prevKeyState & key);
        }
        bool isKeyDown(Key key){
            // prev->0 now->1
            return (keyState & key) && !(prevKeyState & key);
        }
        //get the mouse wheel scroll
        int getMouseWheelScroll(){
            return mouseWheelScroll;
        }
        inline static Entity create(ECS *ecs){
            auto i = ecs->createEntity();
            ecs->addComponent<Input>(i, {});
            return i;
        }
    private:
        //store the key state by bit operation
        uint64_t keyState=0;
        uint64_t prevKeyState=0;
        int mouseWheelScroll = 0;
};

#endif
