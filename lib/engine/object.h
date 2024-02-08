#ifndef OBJECT_H_
#define OBJECT_H_

#include <SDL2/SDL.h>

#include <string>
#include <vector>

namespace _ObjectProperty{
    enum ObjectProperty{
        // will do physical collide
        RIGID = 1<<0,
        // unmovable by collision
        STATIC = 1<<1,
        // is trigger
        TRIGGER = 1<<2,
        // no onCollide
        NO_ONCOLLIDE = 1<<3
    };
}

using ObjectProperty = _ObjectProperty::ObjectProperty;

class CollideShape;
class Object{
    public:
        friend class CollideEngine;
        friend class Scene;
        virtual ~Object(){}
        virtual void update(float dt) = 0;
        virtual void render(SDL_Renderer *renderer) = 0;
        std::string getTag(){
            return tag;
        }
        void setProps(int prop){
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

    private:
        virtual void onCollide(CollideShape *shape){}
        virtual void onTrigger(CollideShape *shape){}
    protected:
        Object(std::string tag, float x, float y, int props)
            :tag(tag),x(x),y(y),props(props){}
        void attachHitbox(CollideShape *shape);
        std::string tag;
        std::vector<CollideShape*> hitboxs;
        float x, y;
        float vx=0, vy=0;
        int props;
};


#endif