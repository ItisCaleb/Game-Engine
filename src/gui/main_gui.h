#ifndef MAIN_GUI_H_
#define MAIN_GUI_H_

#include "gui/gui.h"

class MainGUI: public GUI{
    public:
        void draw();
        static MainGUI* getInstance();
    private:
        inline static MainGUI *instance = 0;

};


#endif