#ifndef PATH_FINDER_H_
#define PATH_FINDER_H_

#include "engine/quad_tree.h"
#include "engine/geomatry.h"

class PathFinder{
    public:
        PathFinder(QuadTree &tree, int width, int gridSize);
        void query(Vector2d start, Vector2d dest, std::vector<Vector2d> &path);
    private:
        void queryObstacles(BoundingBox &box);
        void fillObstacle(BoxCollideShape *shape);
        void fillObstacle(CircleCollideShape *shape);
        void fillObstacle(LineCollideShape *shape);
        void fillObstacle(PointCollideShape *shape);

        QuadTree &tree;
        int gridSize;
        int gridCount;
        std::vector<uint8_t> grid;

        // for searching obstacle
        std::vector<CollideShape*> search;
};

#endif