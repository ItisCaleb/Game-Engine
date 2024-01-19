#include "game.h"

#include "entity/player.h"
#include "utils/resource_manager.h"
void Game::init(SDL_Renderer *renderer, SDL_Window *window, int width, int height){
    if(Game::already_init) return;
    Game::already_init = true;
    Game::renderer = renderer;
    Game::window = window;
    Player* player = new Player();
    Game::entities.push_back(player);
    Game::currentPlayer = player;
    Game::camera = Camera(width, height,2.0f); 
}
Player* Game::getPlayer() {
    return currentPlayer;
}

void Game::setPlayer(Player* player) {
    currentPlayer = player;
}

void Game::setBackground(const std::string& backgroundPath) {
    backgroundTexture = ResourceManager::load<SDL_Texture>(backgroundPath);
}

void Game::renderBackground(SDL_Renderer* renderer) {
    if (backgroundTexture) {
        Camera& cam = getCamera();
        SDL_Rect srcRect = cam.getCameraRect();
        SDL_Rect destRect = {0, 0, cam.view_width, cam.view_height};  // 根据实际窗口大小调整

        SDL_RenderCopy(renderer, backgroundTexture, &destRect, &srcRect);
    }
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

Camera& Game::getCamera() {
    return camera;
}

void Game::render() {
    SDL_SetRenderDrawColor(Game::renderer, 0, 255, 255, 255);
    SDL_RenderClear(Game::renderer);
    renderBackground(Game::renderer);
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