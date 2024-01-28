#ifndef GUI_HELPER_H_
#define GUI_HELPER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
#include "gui/microui.h"
#include "resource/font.h"



class GUIHelper{
    public:
        static void init();
        static void handleInput(SDL_Event &e);
        static void handleRender(SDL_Renderer *renderer);
        static void setCurrentFont(Font *font, int pt=14);
        static mu_Context* getContext();

        // microui wrappers
        static void begin();
        static bool beginWindow(std::string name, SDL_Rect &rect, int opt=0);
        static void bringToFront();
        static bool button(std::string name, int opt=0);
        static void image(SDL_Texture *texture, int w, int h);
        static void label(std::string name);
        static bool textbox(char *buf, size_t len);
        static bool checkbox(std::string name, int *state);
        static bool slider(float *state, float low, float high, float step, const char *fmt="%f", int opt=0);
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