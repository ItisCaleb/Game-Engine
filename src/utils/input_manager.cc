#include "input_manager.h"

InputManager::InputManager() : keyState(0) {}
InputManager::~InputManager() {}

// Singleton pattern
InputManager& InputManager::getInstance() {
    // static variable will only be initialized once
    static InputManager instance;  
    return instance;
}
void InputManager::update() {
    // reset the key state
    keyState = 0;  
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_W]) keyState |= W;
    if (state[SDL_SCANCODE_A]) keyState |= A;
    if (state[SDL_SCANCODE_S]) keyState |= S;
    if (state[SDL_SCANCODE_D]) keyState |= D;
    if (state[SDL_SCANCODE_SPACE]) keyState |= SPACE;
}

bool InputManager::isKeyPressed(Key key) {
    return keyState & key;
}
