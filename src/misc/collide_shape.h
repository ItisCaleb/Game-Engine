#ifndef COLLIDE_SHAPE_H_
#define COLLIDE_SHAPE_H_

#include <SDL2/SDL.h>
#include "object/object.h"

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
    protected:
        CollideShape(ShapeType type):type(type) {};
};

class BoxCollideShape: public CollideShape {
    public:
        BoxCollideShape(float x1, float y1, float x2, float y2)
            : CollideShape(ShapeType::Box), x1(x1), y1(y1), x2(x2), y2(y2) {}
        float x1, y1;
        float x2, y2;
        bool isCollide(CollideShape *shape);
        void render(SDL_Renderer *renderer);
};

class CircleCollideShape : public CollideShape {
    public:
        CircleCollideShape(float x, float y, float r)
            : CollideShape(ShapeType::Circle), x(x), y(y), r(r) {}
        float x, y;
        float r;
        bool isCollide(CollideShape *shape);
        void render(SDL_Renderer *renderer);
};

class LineCollideShape : public CollideShape {
    public:
        LineCollideShape(float x1, float y1, float x2, float y2)
            : CollideShape(ShapeType::Line), x1(x1), y1(y1), x2(x2), y2(y2) {}
        float x1, y1;
        float x2, y2;
        bool isCollide(CollideShape *shape);
        void render(SDL_Renderer *renderer);
};

class PointCollideShape : public CollideShape {
    public:
        PointCollideShape(float x, float y)
            : CollideShape(ShapeType::Point), x(x), y(y) {}
        float x, y;
        bool isCollide(CollideShape *shape);
        void render(SDL_Renderer *renderer);
};

#endif