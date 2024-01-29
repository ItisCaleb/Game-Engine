#ifndef OBJECT_H_
#define OBJECT_H_

#include <string>


class Object{
    public:
        virtual void update(float dt) = 0;
        virtual void render(SDL_Renderer *renderer) = 0;
        std::string getTag(){
            return tag;
        }
        int getFlags(){
            return flags;
        }
    protected:
        Object(std::string tag, int flags)
            :tag(tag),flags(flags){}
        std::string tag;
        int flags;
};


#endif