#include "gui/main_gui.h"

#include "gui/gui_helper.h"

using gh = typename::GUIHelper;

MainGUI* MainGUI::getInstance(){
    if(!MainGUI::instance){
        MainGUI::instance = new MainGUI();
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
        gh::endWindow();
    }
    gh::end();
}