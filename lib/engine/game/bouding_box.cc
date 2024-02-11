#include "engine/bounding_box.h"


static bool checkCollisionBB(BoundingBox *a, BoxCollideShape *b);
static bool checkCollisionBC(BoundingBox *a, CircleCollideShape *b);
static bool checkCollisionBL(BoundingBox *a, LineCollideShape *b);
static bool checkCollisionBP(BoundingBox *a, PointCollideShape *b);
static bool checkCollisionLL(BoundingLine *a, LineCollideShape *b);


bool BoundingBox::isCollide(CollideShape *shape){
    switch (shape->type) {
        case ShapeType::Box:
            return checkCollisionBB(this, reinterpret_cast<BoxCollideShape *>(shape));
        case ShapeType::Circle:
            return checkCollisionBC(this, reinterpret_cast<CircleCollideShape *>(shape));
        case ShapeType::Line:
            return checkCollisionBL(this, reinterpret_cast<LineCollideShape *>(shape));
        case ShapeType::Point:
            return checkCollisionBP(this, reinterpret_cast<PointCollideShape *>(shape));
        default:
            return false;
    }
}

bool checkCollisionBB(BoundingBox *a, BoxCollideShape *b) {
    // two boxes
    float ax = a->x, ay = a->y;
    float bx = b->getRealX(), by = b->getRealY();
    if (ay + a->h < by) return false;
    if (ay > by + b->h) return false;
    if (ax + a->w < bx) return false;
    if (ax > bx + b->w) return false;
    return true;
}

bool checkCollisionBC(BoundingBox *a, CircleCollideShape *b) {
    // get point in rectangle which is close to circle
    // init test = circle center (if circle center is in box)
    float tx = b->getRealX();
    float ty = b->getRealY();
    float ax = a->x, ay = a->y;
    float ax2 = ax + a->w, ay2 = ay + a->h;
    // if circle center is to the right of box take right side
    if (tx < ax)
        tx = ax;
    else if (tx > ax2)
        tx = ax2;
    if (ty < ay)
        ty = ay;
    else if (ty > ay2)
        ty = ay2;
    return dis(b->getRealX(), b->getRealY(), tx, ty) <= b->r;
}
bool checkCollisionBL(BoundingBox *a, LineCollideShape *b) {
    PointCollideShape p1(b->x1, b->y1);
    PointCollideShape p2(b->x2, b->y2);
    if (checkCollisionBP(a, &p1) ||
        checkCollisionBP(a, &p2)) {
        return true;
    }
    float ax = a->x, ay = a->y;
    float ax2 = ax + a->w, ay2 = ay + a->h;
    BoundingLine l1(ax,  ay, ax, ay2);
    BoundingLine l2(ax2, ay, ax2, ay2);
    BoundingLine l3(ax,  ay, ax2, ay);
    BoundingLine l4(ax, ay2, ax2, ay2);
    bool left = checkCollisionLL(&l1, b);
    bool right = checkCollisionLL(&l2, b);
    bool top = checkCollisionLL(&l3, b);
    bool bottom = checkCollisionLL(&l4, b);
    return left || right || top || bottom;
}

bool checkCollisionBP(BoundingBox *a, PointCollideShape *b) {
    float ax = a->x, ay = a->y;
    float bx = b->getRealX(), by = b->getRealY();
    if (ax + a->w < bx) return false;
    if (ax > bx) return false;
    if (ay + a->h < by) return false;
    if (ay > by) return false;
    return true;
}

bool checkCollisionLL(BoundingLine *a, LineCollideShape *b) {
    // calculate the denominator
    float den = ((b->y2 - b->y1) * (a->x2 - a->x1) - (b->x2 - b->x1) * (a->y2 - a->y1));
    // denominator = 0
    if (den == 0) return false;
    float ua = ((b->x2 - b->x1) * (a->y1 - b->y1) -
                (b->y2 - b->y1) * (a->x1 - b->x1)) /
               den;
    float ub = ((a->x2 - a->x1) * (a->y1 - b->y1) -
                (a->y2 - a->y1) * (a->x1 - b->x1)) /
               den;
    // Let 's see if uA and uB tell us the lines are colliding
    return (ua >= 0 && ua <= 1) && (ub >= 0 && ub <= 1);
}