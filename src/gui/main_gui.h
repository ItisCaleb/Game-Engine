#ifndef MAIN_GUI_H_
#define MAIN_GUI_H_

#include <SDL2/SDL_ttf.h>

#include "gui/gui.h"
#include "misc/sprite.h"

class MainGUI: public GUI{
    public:
        void draw();
        static MainGUI* getInstance();
    private:
        inline static MainGUI *instance = 0;
        Sprite *s;
        TTF_Font *font;

};


#endif