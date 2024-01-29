#ifndef MAIN_GUI_H_
#define MAIN_GUI_H_

#include <engine/gui.h>
#include <engine/sprite.h>
#include <engine/font.h>

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