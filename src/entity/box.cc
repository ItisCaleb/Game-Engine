#include "box.h"

#include <SDL2/SDL.h>

#include "utils/resource_manager.h"

Box::Box()
:Entity(640, 360, 50, 50), hitbox(x, y, x+width, y+height), speed(200) , hurtbox(x, y, x+width, y+height){
    this->sprites=(ResourceManager::loadSprites("asset/object/box.png",32,32));
}

Box::~Box() {}

void Box::update(float dt) {

} 

void Box::render(SDL_Renderer *renderer) {
    (*sprites)[0]->render(renderer, this->x, this->y);
    // for(auto sprite: this->sprites){
    //     sprite->render(renderer, this->x, this->y);
    // }
}
