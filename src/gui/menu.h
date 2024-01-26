#ifndef MENU_H_
#define MENU_H_

#include <SDL2/SDL_ttf.h>

#include "gui/gui.h"
#include "misc/sprite.h"

class Menu: public GUI{
    public:
        void draw();
        static Menu* getInstance();
        void onOpen();
        void onClose(){}
    private:
        inline static Menu *instance = 0;
        TTF_Font *font;
        float zoom;


};


#endif