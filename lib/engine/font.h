#ifndef FONT_H_
#define FONT_H_

#include <SDL2/SDL_ttf.h>

class Font{
    public:
        Font(TTF_Font *font, int pt)
            :font(font), pt(pt){}
        ~Font(){
            TTF_CloseFont(font);
        }

        int getPt(){
            return pt;
        }

        TTF_Font* getFont(){
            return font;
        }

    private:
        TTF_Font *font;
        int pt;
};


#endif