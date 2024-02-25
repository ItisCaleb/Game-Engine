#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "engine/ecs/ecs.h"
class Transform: public Component {
    public:
        Transform(){}
        Transform(float x, float y)
            :x(x), y(y), scaleX(1.0f), scaleY(1.0f), flip(false){}        
        float x,y;
        float scaleX, scaleY;
        bool flip;
};

#endif
