#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL2/SDL.h>

class InputManager {
public:
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
    static void update();  
    //check if the key is pressed
    static bool isKeyHold(Key key); 
    static bool isKeyRelease(Key key);
    static bool isKeyDown(Key key);
    //get the mouse wheel scroll
    static int getMouseWheelScroll();
    static void updateMouseWheelScroll(int scroll);
    static void resetMouseWheelScroll();

   private:
    //store the key state by bit operation
    inline static Uint64 keyState=0;
    inline static Uint64 prevKeyState=0;
    inline static int mouseWheelScroll = 0;
};

#endif
