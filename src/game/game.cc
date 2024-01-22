#include "game.h"

#include "entity/player.h"

void Game::init(SDL_Renderer *renderer, SDL_Window *window, int width, int height){
    if (Game::already_init) return;

    Game::already_init = true;
    Game::renderer = renderer;
    Game::window = window;
    Game::width = width;
    Game::height = height;
    Player* player = new Player();
    Game::entities.push_back(player);

    Game::currentPlayer = player;
    Game::camera = Camera(width, height);

}

Player* Game::getPlayer() {
    return currentPlayer;
}

Camera& Game::getCamera() {
    return camera;
}

Scene* Game::getScene() {
    return scene;
}

void Game::setPlayer(Player* player) {
    currentPlayer = player;
}

void Game::setScene(Scene *scene) {
    if(Game::scene){
        delete Game::scene;
    }
    Game::scene = scene;
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
    SDL_SetRenderDrawColor(Game::renderer, 0, 255, 255, 255);
    SDL_RenderClear(Game::renderer);
    Game::scene->render(Game::renderer);
    for (auto e : Game::entities) {
        e->render(Game::renderer);
    }
    SDL_RenderPresent(Game::renderer);
}

SDL_Window* Game::getWindow() {
    return Game::window;
}
SDL_Renderer* Game::getRenderer() {
    return Game::renderer;
}

int Game::getWidth(){
    return Game::width;
}

int Game::getHeight(){
    return Game::height;
}
