#ifndef COLLIDE_SHAPE_H_
#define COLLIDE_SHAPE_H_

#include <SDL2/SDL.h>

enum class ShapeType {
    Box,
    Circle,
    Line,
    Point
};

class CollideShape {
    public:
        const ShapeType type;
    protected:
        CollideShape(ShapeType type) : type(type) {}
};

class BoxCollideShape : protected CollideShape {
    public:
        BoxCollideShape(float x1, float y1, float x2, float y2)
            : CollideShape(ShapeType::Box), x1(x1), y1(y1), x2(x2), y2(y2) {}
        float x1, y1;
        float x2, y2;
};

class CircleCollideShape : protected CollideShape {
    public:
        CircleCollideShape(float x, float y, float r)
            : CollideShape(ShapeType::Circle), x(x), y(y), r(r) {}
        float x, y;
        float r;
};

class LineCollideShape : protected CollideShape {
    public:
        LineCollideShape(float x1, float y1, float x2, float y2)
            : CollideShape(ShapeType::Line), x1(x1), y1(y1), x2(x2), y2(y2) {}
        float x1, y1;
        float x2, y2;
};

class PointCollideShape : protected CollideShape {
    public:
        PointCollideShape(float x, float y)
            : CollideShape(ShapeType::Point), x(x), y(y) {}
        float x, y;
};

#endif