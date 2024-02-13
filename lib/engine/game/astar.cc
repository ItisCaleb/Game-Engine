#include "engine/astar.h"
#include "engine/bounding_box.h"

void AStar::buildGraph(Vector2d start, Vector2d dest){
    float dist = (dest - start).magnitude();
    BoundingBox b = {start.x - dist, start.y - dist, dist * 2, dist * 2};
}

void AStar::query(Vector2d start, Vector2d dest, std::vector<Vector2d> &path){
    
    buildGraph(start, dest);
}