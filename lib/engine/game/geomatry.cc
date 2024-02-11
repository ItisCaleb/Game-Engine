#include "engine/geomatry.h"

#include "engine/collide_shape.h"

template <>
BoxCollideShape minowskiDifference(BoxCollideShape *a, BoxCollideShape *b){
    // the vertical axis is inverted
    BoxCollideShape m(
        a->getRealX() - b->getRealX() - b->w,
        b->getRealY() - a->getRealY() - a->h,
        a->w+b->w,
        a->h+b->h);
    return m;
}



template <>
Vector2d penetrationVector(BoxCollideShape *a){
    float episilon = 0.01f;
    float left = a->offx;
    float top = a->offy;
    float right = a->offx + a->w;
    float bottom = a->offy + a->h;
    float min_dist = fabsf(a->offx);

    Vector2d r(-left + episilon,0);

    
    if(fabsf(right) < min_dist){
        min_dist = fabsf(right);
        r.x = -right - episilon;
        r.y = 0;
    }
    if(fabsf(top) < min_dist){
        min_dist = fabsf(top);
        r.x = 0;
        r.y = top - episilon;
    }
    if (fabsf(bottom) < min_dist) {
		r.x = 0;
		r.y = bottom + episilon;
	}
    return r;
}

float dis(float ax, float ay, float bx, float by){
    return std::sqrt(std::pow(ax - bx, 2) + std::pow(ay - by, 2));
}