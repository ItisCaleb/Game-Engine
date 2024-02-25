#include "gui/main_gui.h"

#include <engine/gui_helper.h>
#include <engine/resource_manager.h>
#include <engine/microui.h>
#include <engine/game.h>
#include <engine/input_manager.h>

#include "object/entity/player.h"

#include "gui/menu.h"

using gh = typename::GUIHelper;

MainGUI* MainGUI::getInstance(){
    if(!MainGUI::instance){
        MainGUI::instance = new MainGUI();
        MainGUI::instance->s = ResourceManager::load<Sprite>("assets/test.png");
        MainGUI::instance->font = ResourceManager::load<Font>("assets/NotoSansTC-Regular.ttf");
    }
    return MainGUI::instance;
}

char buf[256] = {};

void MainGUI::draw(){
    gh::setCurrentFont(font);
    SDL_Rect wrect = {.x=10,.y=10,.w=Game::getWidth(),.h=Game::getHeight()};
    auto ctx = gh::getContext();
    if (gh::beginWindow("My Window", wrect, MU_OPT_NOCLOSE | MU_OPT_NOFRAME | MU_OPT_NOTITLE)) {
        mu_layout_set_next(ctx, mu_rect(30,30,50,50),0);
        if (gh::button("Menu", MU_OPT_ALIGNCENTER)) {
            auto menu = Menu::getInstance();
            if(!menu->isOpened()){
                Game::openGUI(menu);
            }else{
                Game::closeGUI(menu);
            }
        }
        mu_layout_set_next(ctx, mu_rect(100,30,200,50),0);
        gh::setCurrentFont(font, 20);
        char fps[20];
        auto player = dynamic_cast<Player*>(Game::getScene()->getObjectByTag("Player"));
        int state=-1;
        if (player) {
            state = player->comboCnt;
        }
        sprintf(fps, "FPS: %d state: %d", (int)Game::getFPS(),state);
        gh::label(fps);
        gh::endWindow();
    }

}