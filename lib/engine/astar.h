#ifndef ASTAR_H_
#define ASTAR_H_

#include "engine/geomatry.h"

#include <vector>

class AStar{
    public:
        inline static void query(Vector2d start, Vector2d dest, std::vector<Vector2d> &path);
    private:
        inline static void buildGraph(Vector2d start, Vector2d dest);
};

#endif