#include "gui/main_gui.h"

#include "gui/gui_helper.h"
#include "utils/resource_manager.h"

using gh = typename::GUIHelper;

MainGUI* MainGUI::getInstance(){
    if(!MainGUI::instance){
        MainGUI::instance = new MainGUI();
        MainGUI::instance->s = ResourceManager::load<Sprite>("test.png");
    }
    return MainGUI::instance;
}

void MainGUI::draw(){
    gh::begin();
    SDL_Rect wrect = {.x=10,.y=10,.w=300,.h=400};

    if (gh::beginWindow("My Window", wrect)) {
        if (gh::button("My Button")) {
            printf("'My Button' was pressed\n");
        }
        gh::image(this->s->getTexture(),
            this->s->getWidth(),
            this->s->getHeight());
        gh::endWindow();
    }
    gh::end();
}