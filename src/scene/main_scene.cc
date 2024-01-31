#include "scene/main_scene.h"

#include <engine/resource_manager.h>
#include <engine/game.h>
#include <engine/wall.h>
#include <engine/input_manager.h>
#include "gui/main_gui.h"
#include "object/entity/soldier_skeleton.h"


MainScene::MainScene()
:Scene(Game::getWidth(), Game::getHeight()){}

void MainScene::init(){
    this->background = ResourceManager::load<Sprite>("assets/test_background.png");
    auto j = ResourceManager::load<nlohmann::json>("assets/test.json");
    for (auto& element : *j) {
        if(element["type"] == "box"){
            float x1 = element["x1"], y1 = element["y1"];
            float x2 = element["x2"], y2 = element["y2"];
            Wall *w = new Wall(x1,y1, x2-x1,y2-y1,NULL);
            this->addObject(w);
        }
    }
    delete j;

    Player* player = new Player();
    this->addObject(player);
    Skeleton* skeleton = new Skeleton();
    this->addObject(skeleton);
    auto menu = MainGUI::getInstance();
    Game::openGUI(menu);
}

MainScene::~MainScene(){
    delete this->background;
}

void MainScene::update(float dt){
    for (auto o : this->objects) {
        o->update(dt);
    }
    //get player
    auto player = dynamic_cast<Player*>(this->getObjectByTag("Player"));
    if (player) {
        //update camera position
        float zoom = Game::getCamera()->getZoom();
        float x = player->getX() + player->getWidth()/2;
        float y = player->getY() + player->getHeight()/2;
        Game::getCamera()->update(x, y);
    }
}

void MainScene::render(SDL_Renderer* renderer){
    renderBackground(renderer);
    for (auto o : this->objects) {
        o->render(renderer);
    }
    for (auto s : this->shapes) {
        s->render(renderer);
    }
}

void MainScene::destroy(){
    /*for (auto e : this->objects) {
        delete e;
    }*/
}