#ifndef QUAD_TREE_H_
#define QUAD_TREE_H_

#include <SDL2/SDL.h>

#include <vector>
#include <set>

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
        QuadTree(int w, int h, int max_depth, int split_threshold);
        void insert(CollideShape *shape);
        void erase(CollideShape *shape);
        void query(CollideShape *shape, std::vector<CollideShape*> &collides);
        void cleanup();
        std::vector<CollideShape*>& getAllShape();
        void drawGrid(SDL_Renderer *renderer);

    private:
        void findNodes(CollideShape *shape, std::vector<QuadNodeData> &nodes);
        void subDivide(QuadNodeData &data);
        void appendToElements(QuadNodeData &data, int shapdIdx);
        inline void getNodeShapes(int eleIdx, std::vector<CollideShape*> &shape, std::set<int> &pushed){
            for(int i = eleIdx; i!=-1;){
                auto ele = this->elements[i];
                int shapeIdx = ele.shapeIdx;
                i = ele.next;
                if(!pushed.count(shapeIdx)){
                    shape.push_back(this->shapes[shapeIdx]);
                    pushed.insert(shapeIdx);
                }
            }
        }
        FreeList<CollideShape*> shapes;
        FreeList<QuadElement> elements;
        std::vector<QuadNode> nodes;
        BoxCollideShape boundary;
        int free_node = -1;
        int max_depth;
        int split_threshold;

        // for search
        std::vector<QuadNodeData> nodeData;

        std::vector<CollideShape*> leaves;

};

#endif
