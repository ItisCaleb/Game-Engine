#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_

#include "engine/collide_shape.h"
#include "engine/geomatry.h"

class BoundingBox{
    public:
        BoundingBox(float x, float y, float w, float h)
            :x(x),y(y),w(w),h(h){}
        BoundingBox(float w, float h)
            :x(0),y(0),w(w),h(h){}
        float x, y;
        float w, h;
        bool isCollide(CollideShape *shape);
};

class BoundingLine{
    public:
        BoundingLine(float x1, float y1, float x2, float y2)
            :x1(x1),y1(y1),x2(x2),y2(y2){}

        float x1, y1;
        float x2, y2;
};


#endif