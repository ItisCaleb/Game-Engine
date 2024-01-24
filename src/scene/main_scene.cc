#include "scene/main_scene.h"

#include "utils/resource_manager.h"
#include "game/game.h"
#include "object/wall.h"
#include "utils/input_manager.h"
#include "gui/main_gui.h"

MainScene::MainScene()
:Scene(Game::getWidth(), Game::getHeight()){
    this->background = ResourceManager::load<SDL_Texture>("test_background.png");
    auto j = ResourceManager::load<nlohmann::json>("test.json");
    for (auto& element : *j) {
        if(element["type"] == "box"){
            float x1 = element["x1"], y1 = element["y1"];
            float x2 = element["x2"], y2 = element["y2"];
            Wall *w = new Wall(x1,y1, x2-x1,y2-y1,NULL);
            this->objects.push_back(w);
        }
    }
    delete j;
    Player* player = new Player();
    this->objects.push_back(player);
    Game::setPlayer(player);
}

MainScene::~MainScene(){
    delete this->background;
}

void MainScene::update(float dt){
    for (auto o : this->objects) {
        o->update(dt);
    }
    //get player
    Player *player = Game::getPlayer();  
    if (player) {
        //update camera position
        Game::getCamera().update(player->getX(), player->getY());
    }
    auto menu = MainGUI::getInstance();
    if(InputManager::isKeyDown(InputManager::Key::ESC)){
        if(!menu->isOpened())
            Game::openGUI(menu);
        else
            Game::closeGUI(menu);
    }
}

void MainScene::render(SDL_Renderer* renderer){
    renderBackground(renderer);
    for (auto o : this->objects) {
        o->render(renderer);
    }
}

void MainScene::destroy(){
    /*for (auto e : this->objects) {
        delete e;
    }*/
}