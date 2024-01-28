#ifndef GUI_H_
#define GUI_H_

#include <SDL2/SDL.h>

class Game;
class GUI{
    public:
        friend class Game;
        virtual void draw() = 0;
        virtual void onOpen() = 0;
        virtual void onClose() = 0;
        bool isOpened(){
            return this->opened;
        }
    protected:
        bool opened = false;
};



#endif