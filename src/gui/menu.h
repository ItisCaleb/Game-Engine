#ifndef MENU_H_
#define MENU_H_

#include <engine/gui.h>
#include <engine/sprite.h>
#include <engine/font.h>

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