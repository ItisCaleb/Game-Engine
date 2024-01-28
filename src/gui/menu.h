#ifndef MENU_H_
#define MENU_H_

#include "gui/gui.h"
#include "resource/sprite.h"
#include "resource/font.h"

class Menu: public GUI{
    public:
        void draw();
        static Menu* getInstance();
        void onOpen();
        void onClose(){}
    private:
        inline static Menu *instance = 0;
        Font *font;
        float zoom;


};


#endif