#include "input_manager.h"


void InputManager::update() {
    // reset the key state
    prevKeyState = keyState;
    keyState = 0;  
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_W]) keyState |= W;
    if (state[SDL_SCANCODE_A]) keyState |= A;
    if (state[SDL_SCANCODE_S]) keyState |= S;
    if (state[SDL_SCANCODE_D]) keyState |= D;
    if (state[SDL_SCANCODE_SPACE]) keyState |= SPACE;
}

bool InputManager::isKeyHold(Key key) {
    return keyState & key;
}

bool InputManager::isKeyRelease(Key key) {
    // prev->1 now->0
    return !(keyState & key) && (prevKeyState & key);
}

bool InputManager::isKeyDown(Key key) {
    // prev->0 now->1
    return (keyState & key) && !(prevKeyState & key);
}