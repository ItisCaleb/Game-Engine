#ifndef COLLIDE_ENGINE_H_
#define COLLIDE_ENGINE_H_

#include <SDL2/SDL.h>

#include "engine/collide_shape.h"
#include "engine/quad_tree.h"
#include <vector>
#include <utility>

class CollideEngine{
    public:
        CollideEngine(int w, int h)
            :tree(w,h,8){}
        void handle(float dt);
        // add shape to collision detection
        void addCollideShape(CollideShape *shape);
        void adjustObject(Object *object);

        void drawShapes(SDL_Renderer *renderer);
        
        QuadTree tree;
        std::vector<CollideShape*> shapes;
};

#endif