#ifndef COLLIDE_ENGINE_H_
#define COLLIDE_ENGINE_H_

#include <SDL2/SDL.h>

#include "engine/collide_shape.h"
#include <vector>
#include <utility>

class CollideEngine{
    public:
        void handleRigid(float dt);
        // add shape to collision detection
        void addCollideShape(CollideShape *shape);


        // get all shape collided with this shape
        void getCollided(CollideShape *shape, std::vector<CollideShape*> &vec);
        void drawShapes(SDL_Renderer *renderer);
    private:
        void searchCollides();
        
        std::vector<std::pair<CollideShape*, CollideShape*>> collides; 
        std::vector<CollideShape*> shapes;
        std::vector<CollideShape*> rigidShapes;
};

#endif