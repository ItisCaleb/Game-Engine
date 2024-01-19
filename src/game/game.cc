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

void Game::renderBackground(SDL_Renderer* renderer, const Camera& cam) {
    if (backgroundTexture) {
        // 获取背景图像原始尺寸
        int bgWidth, bgHeight;
        SDL_QueryTexture(backgroundTexture, nullptr, nullptr, &bgWidth, &bgHeight);

        // 计算摄像头视口应该显示的背景区域
        float zoom = camera.getZoom();
        SDL_Rect srcRect;
        srcRect.w = static_cast<int>(camera.view_width / zoom);
        srcRect.h = static_cast<int>(camera.view_height / zoom);
        srcRect.x = static_cast<int>(camera.getX() - srcRect.w / 2);
        srcRect.y = static_cast<int>(camera.getY() - srcRect.h / 2);

        // 调整源矩形以确保不超出背景图像边界
        srcRect.x = std::max(0, std::min(srcRect.x, bgWidth - srcRect.w));
        srcRect.y = std::max(0, std::min(srcRect.y, bgHeight - srcRect.h));

        // 目标矩形覆盖整个屏幕
        SDL_Rect destRect = {0, 0, camera.view_width, camera.view_height};

        // 渲染背景
        SDL_RenderCopy(renderer, backgroundTexture, &srcRect, &destRect);
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
    renderBackground(Game::renderer, camera);
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