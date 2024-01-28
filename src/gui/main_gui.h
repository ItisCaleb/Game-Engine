#ifndef MAIN_GUI_H_
#define MAIN_GUI_H_

#include "gui/gui.h"
#include "resource/sprite.h"
#include "resource/font.h"

class MainGUI: public GUI{
    public:
        void draw();
        static MainGUI* getInstance();
        void onOpen(){}
        void onClose(){}
    private:
        inline static MainGUI *instance = 0;
        Sprite *s;
        Font *font;

};


#endif