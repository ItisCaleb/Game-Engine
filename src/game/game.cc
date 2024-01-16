#include "game.h"

#include "entity/player.h"

void Game::init(SDL_Renderer *renderer, SDL_Window *window, int width, int height){
    if(Game::already_init) return;
    Game::already_init = true;
    Game::renderer = renderer;
    Game::window = window;
    Game::entities.push_back(new Player());
}

void Game::destroy() {
    SDL_DestroyRenderer(Game::renderer);
    SDL_DestroyWindow(Game::window);
    for (auto e : Game::entities) {
        delete e;
    }
}

void Game::update(float dt) {
    for (auto e : Game::entities) {
        e->update(dt);
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(Game::renderer, 210, 190, 170, 255);
    SDL_RenderClear(Game::renderer);
    for (auto e : Game::entities) {
        e->render(Game::renderer);
    }
    SDL_RenderPresent(Game::renderer);
}

SDL_Window* Game::getWindow(){
    return Game::window;
}
SDL_Renderer* Game::getRenderer(){
    return Game::renderer;
}