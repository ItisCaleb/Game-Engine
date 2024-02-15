#include "engine/path_finder.h"

PathFinder::PathFinder(QuadTree &tree, int width, int gridSize)
:tree(tree),gridSize(gridSize), gridCount(width/gridSize){
    grid.reserve(gridCount * gridCount);
}

void PathFinder::query(Vector2d start, Vector2d dest, std::vector<Vector2d> &path){
    /*float dist = (dest - start).magnitude();
    BoundingBox b = {start.x - dist, start.y - dist, dist * 2, dist * 2};
    this->queryObstacles(b);*/
    path.push_back(dest);
}

void PathFinder::queryObstacles(BoundingBox &box){
    this->search.clear();
    
    tree.query(&box, this->search);
    for(auto shape: this->search){
        auto obj = shape->getObject();
        if(!(obj->getProps() & ObjectProperty::RIGID)) continue; 
        switch (shape->type){
        case ShapeType::Box:
            this->fillObstacle(reinterpret_cast<BoxCollideShape*>(shape));
            break;
        case ShapeType::Circle:
            this->fillObstacle(reinterpret_cast<CircleCollideShape*>(shape));
            break;
        case ShapeType::Line:
            this->fillObstacle(reinterpret_cast<LineCollideShape*>(shape));
            break;
        case ShapeType::Point:
            this->fillObstacle(reinterpret_cast<PointCollideShape*>(shape));
            break;
        default:
            break;
        }
    }
}

void PathFinder::fillObstacle(BoxCollideShape *shape){

}

void PathFinder::fillObstacle(CircleCollideShape *shape){

}

void PathFinder::fillObstacle(LineCollideShape *shape){

}

void PathFinder::fillObstacle(PointCollideShape *shape){
    int x = shape->getRealX() / this->gridSize;
    int y = shape->getRealY() / this->gridSize;
    this->grid[x + y*gridCount] = 1;
}
