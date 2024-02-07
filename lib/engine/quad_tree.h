#ifndef QUAD_TREE_H_
#define QUAD_TREE_H_

#include <SDL2/SDL.h>

#include <vector>

#include "engine/collide_shape.h"
#include "engine/freelist.h"


struct QuadElement{
    // points to next element if this is a branch
    // else -1
    int next;

    // set to -1 if this is a branch
    // else it will represent element count
    int shapeIdx;
};

struct QuadNode{
    // points to first child if this is a branch
    // else first element
    // if no element then -1
    int next;

    // set to -1 if this is a branch
    // else it will represent element count
    int count;
};


// use in node find
struct QuadNodeData{
    BoxCollideShape boundary;
    int nodeIdx;
    int depth;
};

class QuadTree{
    public:
        QuadTree(int w, int h, int max_depth);
        void insert(CollideShape *shape);
        void erase(CollideShape *shape);
        void query(CollideShape *shape, std::vector<CollideShape*> &collides);
        void drawGrid(SDL_Renderer *renderer);

    private:
        void findNodes(CollideShape *shape, std::vector<QuadNodeData> &nodes);
        void subDivide(QuadNodeData &data);
        void appendToElements(QuadNodeData &data, int shapdIdx);
        FreeList<CollideShape*> shapes;
        FreeList<QuadElement> elements;
        std::vector<QuadNode> nodes;
        BoxCollideShape boundary;
        int max_depth;

        // for search
        std::vector<QuadNodeData> nodeData;

};

#endif
