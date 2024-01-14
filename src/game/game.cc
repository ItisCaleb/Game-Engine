#include "game.h"

#include "entity/player.h"

Game::Game(SDL_Renderer *renderer, int width, int height)
    : renderer(renderer) {
    this->entities.push_back(new Player());
}

Game::~Game() {
}

void Game::update(float dt) {
    return;
}

void Game::render() {
    SDL_SetRenderDrawColor(this->renderer, 0, 255, 255, 255);
    SDL_RenderClear(this->renderer);
    for (auto e : this->entities) {
        e->render(this->renderer);
    }
    SDL_RenderPresent(this->renderer);
}
