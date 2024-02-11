#ifndef COLLIDE_SHAPE_H_
#define COLLIDE_SHAPE_H_

#include <SDL2/SDL.h>

#include "engine/object.h"

enum class ShapeType {
    Box,
    Circle,
    Line,
    Point
};

class CollideShape {
    public:
        friend class Object;
        const ShapeType type;
        virtual bool isCollide(CollideShape *shape) = 0;
        virtual void render(SDL_Renderer *renderer) = 0;
        Object *getObject(){
            return this->object;
        }
    protected:
        Object *object = nullptr;
        CollideShape(ShapeType type)
            :type(type) {};
};

class BoxCollideShape: public CollideShape {
    public:
        BoxCollideShape(float offx, float offy, float w, float h)
            : CollideShape(ShapeType::Box), offx(offx), offy(offy), w(w), h(h) {}
        BoxCollideShape(float w, float h)
            : CollideShape(ShapeType::Box), offx(0), offy(0), w(w), h(h) {}
        float offx, offy;
        float w, h;
        bool isCollide(CollideShape *shape);
        void render(SDL_Renderer *renderer);
        float getRealX(){
            if(this->object)
                return this->object->getX() + offx;
            else
                return offx;
        }
        float getRealY(){
            if(this->object)
                return this->object->getY() + offy;
            else return offy;
        }
};

class CircleCollideShape : public CollideShape {
    public:
        CircleCollideShape(float x, float y, float r)
            : CollideShape(ShapeType::Circle), offx(x), offy(y), r(r) {}
        CircleCollideShape(float r)
            : CollideShape(ShapeType::Circle), offx(0), offy(0), r(r) {}
        float offx, offy;
        float r;
        bool isCollide(CollideShape *shape);
        void render(SDL_Renderer *renderer);
                float getRealX(){
            if(this->object)
                return this->object->getX() + offx;
            else
                return offx;
        }
        float getRealY(){
            if(this->object)
                return this->object->getY() + offy;
            else return offy;
        }
};

class LineCollideShape : public CollideShape {
    public:
        LineCollideShape(float x1, float y1, float x2, float y2)
            : CollideShape(ShapeType::Line), x1(x1), y1(y1), x2(x2), y2(y2) {}
        float x1, y1;
        float x2, y2;
        bool isCollide(CollideShape *shape);
        void render(SDL_Renderer *renderer);
        void update(float x1, float y1, float x2, float y2){
            this->x1 = x1;
            this->y1 = y1;
            this->x2 = x2;
            this->y2 = y2;
        }
};

class PointCollideShape : public CollideShape {
    public:
        PointCollideShape(float x, float y)
            : CollideShape(ShapeType::Point), offx(x), offy(y) {}
        PointCollideShape(Object *object)
            : CollideShape(ShapeType::Point), offx(0), offy(0) {}
        float offx, offy;
        bool isCollide(CollideShape *shape);
        void render(SDL_Renderer *renderer);
                float getRealX(){
            if(this->object)
                return this->object->getX() + offx;
            else
                return offx;
        }
        float getRealY(){
            if(this->object)
                return this->object->getY() + offy;
            else return offy;
        }

};

#endif