#ifndef ASTAR_H_
#define ASTAR_H_

#include "engine/geomatry.h"

#include <vector>

class AStar{
    public:
        inline static void query(std::vector<uint8_t> graph,
        Vector2d start, Vector2d dest, std::vector<Vector2d> &path);
};

#endif