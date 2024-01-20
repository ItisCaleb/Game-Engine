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
    Game::camera = Camera(width, height, 5.0f);

}

Player* Game::getPlayer() {
    return currentPlayer;
}

Camera& Game::getCamera() {
    return camera;
}

void Game::setPlayer(Player* player) {
    currentPlayer = player;
}

void Game::loadBackground(const std::string& backgroundPath) {
    backgroundTexture = ResourceManager::load<SDL_Texture>(backgroundPath);
}

void Game::renderBackground(SDL_Renderer* renderer, const Camera& cam) {
    if (backgroundTexture) {
        // get background texture size
        int bgWidth, bgHeight;
        SDL_QueryTexture(backgroundTexture, nullptr, nullptr, &bgWidth, &bgHeight);

        // set source rect to cover the whole background
        SDL_Rect srcRect;
        srcRect.w = Game::width;
        srcRect.h = Game::height;
        srcRect.x = (int)camera.getX();
        srcRect.y = (int)camera.getY();

        //check if source rect is out of bounds
        srcRect.x = std::max(0, std::min(srcRect.x, bgWidth - srcRect.w));
        srcRect.y = std::max(0, std::min(srcRect.y, bgHeight - srcRect.h));

        // set destination rect to cover the whole screen
        SDL_Rect destRect = {0, 0, Game::width, Game::height};
        // apply camera
        destRect = camera.apply( destRect);
        // render
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

void Game::render() {
    SDL_SetRenderDrawColor(Game::renderer, 0, 255, 255, 255);
    SDL_RenderClear(Game::renderer);
    renderBackground(Game::renderer, camera);
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

