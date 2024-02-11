#ifndef GEOMATRY_H_
#define GEOMATRY_H_

#include <cmath>

class Vector2d{
    public:
        Vector2d(float x, float y)
            :x(x),y(y){}
        float x,y;
        Vector2d add(Vector2d &v){
            return Vector2d(x+v.x, y+v.y);
        }
        Vector2d sub(Vector2d &v){
            return Vector2d(x-v.x, y-v.y);
        }
        float dot(Vector2d &v){
            return x*v.x + y*v.y;
        }
};

template <class T>
T minowskiDifference(T *a, T *b);

template <class T>
Vector2d penetrationVector(T *a);

float dis(float ax, float ay, float bx, float by);


#endif