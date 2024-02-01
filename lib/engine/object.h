#ifndef OBJECT_H_
#define OBJECT_H_

#include <string>

namespace _ObjectProperty{
    enum ObjectProperty{
        // will do physical collide
        RIGID = 1,
        // unmovable by collision
        STATIC = 2,
    };
}

using ObjectProperty = _ObjectProperty::ObjectProperty;

class Object{
    public:
        virtual void update(float dt) = 0;
        virtual void render(SDL_Renderer *renderer) = 0;
        std::string getTag(){
            return tag;
        }
        void setFlags(int prop){
            this->props |= prop;
        }
        int getProps(){
            return props;
        }
        void setXY(float x, float y){
            this->x = x;
            this->y = y;
        }
        void setX(float x){ this->x = x; }
        void setY(float y){ this->y = y; }
        float getX(){ return x; }
        float getY(){ return y; }
        
        void setVelocityXY(float vx, float vy){
            this->vx = vx;
            this->vy = vy;
        }
        void setVelocityX(float vx){ this->vx = vx;}
        void setVelocityY(float vy){ this->vy = vy;}

        float getVelocityX(){ return vx; }
        float getVelocityY(){ return vy; }
    protected:
        Object(std::string tag, float x, float y, int props)
            :tag(tag),x(x),y(y),props(props){}
        std::string tag;
        float x, y;
        float vx, vy;
        int props;
};


#endif