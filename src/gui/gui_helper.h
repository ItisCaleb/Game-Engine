#ifndef GUI_HELPER_H_
#define GUI_HELPER_H_

#include <SDL2/SDL.h>

#include <string>
#include "gui/microui.h"

class GUIHelper{
    public:
        static void init();
        static void handleInput(SDL_Event &e);
        static void handleRender(SDL_Renderer *renderer);

        // microui wrappers
        static void begin();
        static bool beginWindow(std::string name, SDL_Rect &rect);
        static bool button(std::string name);
        static void endWindow();
        static void end();
    private:
        inline static mu_Context *ctx = 0;
};

#endif