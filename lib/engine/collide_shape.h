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
        const ShapeType type;
        virtual bool isCollide(CollideShape *shape) = 0;
        virtual void render(SDL_Renderer *renderer) = 0;
        Object *getObject(){
            return this->object;
        }
    private:
        Object *object;
    protected:
        CollideShape(ShapeType type, Object *object)
            :type(type),object(object) {};
};

class BoxCollideShape: public CollideShape {
    public:
        BoxCollideShape(float x, float y, float w, float h, Object *object)
            : CollideShape(ShapeType::Box, object), x(x), y(y), w(w), h(h) {}
        float x, y;
        float w, h;
        bool isCollide(CollideShape *shape);
        void render(SDL_Renderer *renderer);
        void update(float x, float y){
            this->x = x;
            this->y = y;
        }
};

class CircleCollideShape : public CollideShape {
    public:
        CircleCollideShape(float x, float y, float r, Object *object)
            : CollideShape(ShapeType::Circle, object), x(x), y(y), r(r) {}
        float x, y;
        float r;
        bool isCollide(CollideShape *shape);
        void render(SDL_Renderer *renderer);
        void update(float x, float y){
            this->x = x;
            this->y = y;
        }
};

class LineCollideShape : public CollideShape {
    public:
        LineCollideShape(float x1, float y1, float x2, float y2, Object *object)
            : CollideShape(ShapeType::Line, object), x1(x1), y1(y1), x2(x2), y2(y2) {}
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
        PointCollideShape(float x, float y, Object *object)
            : CollideShape(ShapeType::Point, object), x(x), y(y) {}
        float x, y;
        bool isCollide(CollideShape *shape);
        void render(SDL_Renderer *renderer);
        void update(float x, float y){
            this->x = x;
            this->y = y;
        }

};

#endif