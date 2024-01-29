#include "engine/timer.h"

#include <SDL2/SDL.h>


void Timer::start(){
    this->startTicks = SDL_GetTicks();
    this->paused = false;
}

void Timer::restart(){
    this->startTicks = SDL_GetTicks();
}

void Timer::pause(){
    this->pauseTicks = SDL_GetTicks();
    this->paused = true;
}

int Timer::getTicks(){
    if(this->paused){
        return pauseTicks - startTicks;
    }else{
        return SDL_GetTicks() - startTicks;
    }
}
int Timer::getTicksAndRestart(){
    int ticks = getTicks();
    this->restart();
    return ticks;
}