#ifndef COLLIDE_SHAPE_H_
#define COLLIDE_SHAPE_H_

#include <SDL2/SDL.h>

enum ShapeType{
    Box, Circle, Line, Point
};

class CollideShape {
    public:
        const ShapeType type;
    protected:
        CollideShape(ShapeType type):type(type){}
};

class BoxCollideShape: protected CollideShape
{
private:
    int x1, y1;
    int x2, y2;
public:
    BoxCollideShape(int x1, int y1, int x2, int y2)
        :CollideShape(ShapeType::Box),x1(x1),y1(y1),x2(x2),y2(y2){}
};

class CircleCollideShape: protected CollideShape
{
private:
    int x, y;
    int r;
public:
    CircleCollideShape(int x, int y, int r)
        :CollideShape(ShapeType::Circle),x(x),y(y),r(r){}
};

class LineCollideShape: protected CollideShape
{
private:
    int x1, y1;
    int x2, y2;
public:
    LineCollideShape(int x1, int y1, int x2, int y2)
        :CollideShape(ShapeType::Line),x1(x1),y1(y1),x2(x2),y2(y2){}
};

class PointCollideShape: protected CollideShape
{
private:
    int x, y;
public:
    PointCollideShape(int x, int y)
        :CollideShape(ShapeType::Point),x(x),y(y){}
};


#endif