#ifndef RIGID_BODY_COMPONENT_H
#define RIGID_BODY_COMPONENT_H

#include "engine/ecs/ecs.h"
#include "engine/misc/collide_shape.h"

class RigidBody: public Component {
    public:
        RigidBody(){}    
        float vx, vy;
        float ax, ay;
        CollideShape *shape;
        
};

#endif
