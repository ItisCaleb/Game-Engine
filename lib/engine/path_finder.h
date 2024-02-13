#ifndef PATH_FINDER_H_
#define PATH_FINDER_H_

#include "engine/quad_tree.h"

class PathFinder{
    PathFinder(QuadTree &tree);
    private:
        QuadTree &tree;
};

#endif