#include "scene/main_scene.h"

#include "utils/resource_manager.h"
#include "game/game.h"

MainScene::MainScene()
:Scene(Game::getWidth(), Game::getHeight()){
    this->background = ResourceManager::load<SDL_Texture>("test_background.png");
    this->loadScene("test.json");
}

MainScene::~MainScene(){
    delete this->background;
}

void MainScene::render(SDL_Renderer* renderer){
    renderBackground(renderer);
    //get player
    Player *player = Game::getPlayer();  
    if (player) {
        //update camera position
        Game::getCamera().update(player->getX(), player->getY());
    }
}