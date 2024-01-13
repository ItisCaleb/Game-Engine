#include "game.h"

Game::Game(SDL_Renderer *renderer)
:renderer(renderer){


}

Game::~Game(){


}

void Game::update(float dt){

    return;
}

void Game::render(){
    SDL_SetRenderDrawColor(this->renderer, 0, 255, 255, 255);
    SDL_RenderClear(this->renderer);

    SDL_RenderPresent(this->renderer);
}