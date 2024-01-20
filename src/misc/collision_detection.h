#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

#include "misc/collide_shape.h"
#include <SDL2/SDL.h>
#include <cmath>

// enum class ShapeType {
//     Box,
//     Circle,
//     Line,
//     Point
// };
// BB BC BL BP 
// CC CL CP
// LL LP
// PP

class CollisionDetection{
    public:
        float dis(float ax,float ay,float bx,float by);
        bool checkCollisionBB(BoxCollideShape a,BoxCollideShape b);
        bool checkCollisionBC(BoxCollideShape a,CircleCollideShape b);
        bool checkCollisionBL(BoxCollideShape a,LineCollideShape b);
        bool checkCollisionBP(BoxCollideShape a,PointCollideShape b);
        bool checkCollisionCC(CircleCollideShape a,CircleCollideShape b);
        bool checkCollisionCP(CircleCollideShape a,PointCollideShape b);
        bool checkCollisionCL(CircleCollideShape a,LineCollideShape b);
        bool checkCollisionLL(LineCollideShape a,LineCollideShape b);
        bool checkCollisionLP(LineCollideShape a,PointCollideShape b);
        bool checkCollisionPP(PointCollideShape a,PointCollideShape b);
};

#endif