#ifndef QUAD_TREE_H_
#define QUAD_TREE_H_

#include <vector>

#include "engine/collide_shape.h"

struct QuadElement{
    // points to next element
    // if this is the end, then set to -1
    int nextIdx;
    CollideShape *shape;
};

struct QuadNode{
    // points to first child if this is a branch
    // else it will point to first element
    int first_child;

    // set to -1 if this is a branch
    // else it will represent element count
    int count;
};

class QuadTree{
    public:
        QuadTree(int w, int h, int max_depth);
        void insert(CollideShape *shape);
        void erase(CollideShape *shape);
        void query(SDL_FRect &region, std::vector<CollideShape*> collides);

    private:
        std::vector<QuadElement> shapes;
        std::vector<QuadNode> nodes;
        SDL_FRect boundary;
        int max_depth;

};

#endif
