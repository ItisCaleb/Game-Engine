#include "gui/menu.h"

#include "gui/gui_helper.h"
#include "utils/resource_manager.h"
#include "gui/microui.h"
#include "game/game.h"

using gh = typename::GUIHelper;

Menu* Menu::getInstance(){
    if(!Menu::instance){
        Menu::instance = new Menu();
        Menu::instance->font = ResourceManager::load<Font>("assets/NotoSansTC-Regular.ttf");
    }
    return Menu::instance;
}

void Menu::onOpen(){
    this->zoom = Game::getCamera()->getTargetZoom();
}

char buff[256] = {};

void Menu::draw(){
    gh::setCurrentFont(font);
    SDL_Rect wrect = {.x=10,.y=100,.w=300,.h=300};
    auto ctx = gh::getContext();
    if (gh::beginWindow("Menu", wrect, MU_OPT_NOCLOSE)) {
        gh::bringToFront();
        gh::label("CameraZoom");
        if(gh::slider(&this->zoom, 0.5f, 3.0f, 0.01f)){
            Game::getCamera()->updateZoom(this->zoom);
        }
        gh::textbox(buff, 256);
        gh::endWindow();
    }
}