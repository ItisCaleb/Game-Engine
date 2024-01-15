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
        SPACE = 16 
    };
    //get the instance of InputManager
    static InputManager& getInstance();  
    // forbid copy constructor
    InputManager(const InputManager&) = delete;           
    // forbid assignment operator
    InputManager& operator=(const InputManager&) = delete; 
    InputManager();
    ~InputManager();
    //update the key state
    void update();  
    //check if the key is pressed
    bool isKeyPressed(Key key); 

private:
    //store the key state by bit operation
    long long keyState;  
};

#endif
