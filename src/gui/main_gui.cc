#include "gui/main_gui.h"

#include "gui/gui_helper.h"
#include "utils/resource_manager.h"
#include "gui/microui.h"
#include "game/game.h"

using gh = typename::GUIHelper;

MainGUI* MainGUI::getInstance(){
    if(!MainGUI::instance){
        MainGUI::instance = new MainGUI();
        MainGUI::instance->s = ResourceManager::load<Sprite>("test.png");
        MainGUI::instance->font = ResourceManager::load<TTF_Font>("NotoSansTC-Regular.ttf");
    }
    return MainGUI::instance;
}

char buf[256] = {};

void MainGUI::draw(){
    gh::begin();
    gh::setCurrentFont(font);
    SDL_Rect wrect = {.x=10,.y=10,.w=Game::getWidth(),.h=Game::getHeight()};
    auto ctx = gh::getContext();
    
    if (gh::beginWindow("My Window", wrect, MU_OPT_NOCLOSE | MU_OPT_NOFRAME | MU_OPT_NOTITLE)) {
        mu_layout_set_next(ctx, mu_rect(30,30,50,50),0);
        if (gh::button("Menu", MU_OPT_ALIGNCENTER)) {
            printf("'My Button' was pressed\n");
        }
        gh::endWindow();
    }
    gh::end();
}