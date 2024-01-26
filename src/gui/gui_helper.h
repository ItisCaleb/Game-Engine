#ifndef GUI_HELPER_H_
#define GUI_HELPER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
#include "gui/microui.h"



class GUIHelper{
    public:
        static void init();
        static void handleInput(SDL_Event &e);
        static void handleRender(SDL_Renderer *renderer);
        static void setCurrentFont(TTF_Font *font);
        static mu_Context* getContext();

        // microui wrappers
        static void begin();
        static bool beginWindow(std::string name, SDL_Rect &rect, int opt=0);
        static bool button(std::string name, int opt=0);
        static bool image(SDL_Texture *texture, int w, int h);
        static bool textbox(char *buf, size_t len);
        static void endWindow();
        static void end();
    private:
        inline static mu_Context *ctx = 0;
#ifdef _WIN32
        inline static HWND hwnd;
        inline static HIMC himc;
#endif
};

#endif