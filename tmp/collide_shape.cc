#include "engine/collide_shape.h"

#include "engine/game.h"
#include "engine/geomatry.h"

inline static bool checkCollisionBB(BoxCollideShape *a, BoxCollideShape *b);
inline static bool checkCollisionBC(BoxCollideShape *a, CircleCollideShape *b);
inline static bool checkCollisionBL(BoxCollideShape *a, LineCollideShape *b);
inline static bool checkCollisionBP(BoxCollideShape *a, PointCollideShape *b);
inline static bool checkCollisionCC(CircleCollideShape *a, CircleCollideShape *b);
inline static bool checkCollisionCP(CircleCollideShape *a, PointCollideShape *b);
inline static bool checkCollisionCL(CircleCollideShape *a, LineCollideShape *b);
inline static bool checkCollisionLL(LineCollideShape *a, LineCollideShape *b);
inline static bool checkCollisionLP(LineCollideShape *a, PointCollideShape *b);
inline static bool checkCollisionPP(PointCollideShape *a, PointCollideShape *b);

bool BoxCollideShape::isCollide(CollideShape *shape) {
    return shape->isCollide(this);
}
bool BoxCollideShape::isCollide(BoxCollideShape *shape){
    return checkCollisionBB(this, shape);
}
bool BoxCollideShape::isCollide(CircleCollideShape *shape){
    return checkCollisionBC(this, shape);
}
bool BoxCollideShape::isCollide(LineCollideShape *shape){
    return checkCollisionBL(this, shape);
}
bool BoxCollideShape::isCollide(PointCollideShape *shape){
    return checkCollisionBP(this, shape);
}


void BoxCollideShape::render(Renderer *renderer) {
    renderer->SetRenderDrawColor(0xFF, 0x00, 0x00, 0xFF);
    SDL_FRect rect = {.x = this->getRealX(), .y = this->getRealY(), .w = this->w, .h = this->h};
    renderer->RenderDrawRectF(&rect);
}

bool CircleCollideShape::isCollide(CollideShape *shape) {
    return shape->isCollide(this);
}

bool CircleCollideShape::isCollide(BoxCollideShape *shape){
    return checkCollisionBC(shape, this);
}
bool CircleCollideShape::isCollide(CircleCollideShape *shape){
    return checkCollisionCC(this, shape);
}
bool CircleCollideShape::isCollide(LineCollideShape *shape){
    return checkCollisionCL(this, shape);
}
bool CircleCollideShape::isCollide(PointCollideShape *shape){
    return checkCollisionCP(this, shape);
}

void CircleCollideShape::render(Renderer *renderer) {
    renderer->SetRenderDrawColor(0xFF, 0x00, 0x00, 0xFF);    
    const float diameter = (this->r * 2);

    float x = (r - 1);
    float y = 0;
    float tx = 1;
    float ty = 1;
    float error = (tx - diameter);
    float ax = Game::getCamera()->applyX(this->getRealX());
    float ay = Game::getCamera()->applyY(this->getRealY());
    while (x >= y) {
        //  Each of the following renders an octant of the circle
        renderer->RenderDrawPointF(ax + x, ay - y);
        renderer->RenderDrawPointF(ax + x, ay + y);
        renderer->RenderDrawPointF(ax - x, ay - y);
        renderer->RenderDrawPointF(ax - x, ay + y);
        renderer->RenderDrawPointF(ax + y, ay - x);
        renderer->RenderDrawPointF(ax + y, ay + x);
        renderer->RenderDrawPointF(ax - y, ay - x);
        renderer->RenderDrawPointF(ax - y, ay + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

bool LineCollideShape::isCollide(CollideShape *shape) {
    return shape->isCollide(this);
}

bool LineCollideShape::isCollide(BoxCollideShape *shape){
    return checkCollisionBL(shape, this);
}
bool LineCollideShape::isCollide(CircleCollideShape *shape){
    return checkCollisionCL(shape, this);
}
bool LineCollideShape::isCollide(LineCollideShape *shape){
    return checkCollisionLL(this, shape);
}
bool LineCollideShape::isCollide(PointCollideShape *shape){
    return checkCollisionLP(this, shape);
}

void LineCollideShape::render(Renderer *renderer) {
    renderer->SetRenderDrawColor(0xFF, 0x00, 0x00, 0xFF);
    renderer->RenderDrawLineF(x1, y1, x2, y2);
}

bool PointCollideShape::isCollide(CollideShape *shape) {
    return shape->isCollide(this);
}

bool PointCollideShape::isCollide(BoxCollideShape *shape){
    return checkCollisionBP(shape, this);
}
bool PointCollideShape::isCollide(CircleCollideShape *shape){
    return checkCollisionCP(shape, this);
}
bool PointCollideShape::isCollide(LineCollideShape *shape){
    return checkCollisionLP(shape, this);
}
bool PointCollideShape::isCollide(PointCollideShape *shape){
    return checkCollisionPP(this, shape);
}

void PointCollideShape::render(Renderer *renderer) {
    renderer->SetRenderDrawColor(0xFF, 0x00, 0x00, 0xFF);
    float ax = Game::getCamera()->applyX(this->getRealX());
    float ay = Game::getCamera()->applyY(this->getRealY());
    renderer->RenderDrawPointF(ax, ay);
}

bool checkCollisionBB(BoxCollideShape *a, BoxCollideShape *b) {
    // two boxes
    float ax = a->getRealX(), ay = a->getRealY();
    float bx = b->getRealX(), by = b->getRealY();
    if (ay + a->h < by) return false;
    if (ay > by + b->h) return false;
    if (ax + a->w < bx) return false;
    if (ax > bx + b->w) return false;
    return true;
}
bool checkCollisionBC(BoxCollideShape *a, CircleCollideShape *b) {
    // get point in rectangle which is close to circle
    // init test = circle center (if circle center is in box)
    float tx = b->getRealX();
    float ty = b->getRealY();
    float ax = a->getRealX(), ay = a->getRealY();
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
bool checkCollisionBL(BoxCollideShape *a, LineCollideShape *b) {
    PointCollideShape p1(b->x1, b->y1);
    PointCollideShape p2(b->x2, b->y2);
    if (checkCollisionBP(a, &p1) ||
        checkCollisionBP(a, &p2)) {
        return true;
    }
    float ax = a->getRealX(), ay = a->getRealY();
    float ax2 = ax + a->w, ay2 = ay + a->h;
    LineCollideShape l1(ax,  ay, ax, ay2);
    LineCollideShape l2(ax2, ay, ax2, ay2);
    LineCollideShape l3(ax,  ay, ax2, ay);
    LineCollideShape l4(ax, ay2, ax2, ay2);
    bool left   = checkCollisionLL(&l1, b);
    bool right  = checkCollisionLL(&l2, b);
    bool top    = checkCollisionLL(&l3, b);
    bool bottom = checkCollisionLL(&l4, b);
    return left || right || top || bottom;
}

bool checkCollisionBP(BoxCollideShape *a, PointCollideShape *b) {
    float ax = a->getRealX(), ay = a->getRealY();
    float bx = b->getRealX(), by = b->getRealY();
    if (ax + a->w < bx) return false;
    if (ax > bx) return false;
    if (ay + a->h < by) return false;
    if (ay > by) return false;
    return true;
}


bool checkCollisionCC(CircleCollideShape *a, CircleCollideShape *b) {
    // distance between two points <=  (ra + rb)
    float ax = a->getRealX(), ay = a->getRealY();
    float bx = b->getRealX(), by = b->getRealY();
    return dis(ax, ay, bx, by) <= (a->r + b->r);
}

bool checkCollisionCP(CircleCollideShape *a, PointCollideShape *b) {
    // distance between circle and points <=  r
    float ax = a->getRealX(), ay = a->getRealY();
    float bx = b->getRealX(), by = b->getRealY();
    return dis(ax, ay, bx, by) <= (a->r);
}
bool checkCollisionCL(CircleCollideShape *a, LineCollideShape *b) {
    // line points in circle
    float ax = a->getRealX(), ay = a->getRealY(), ar = a->r;
    float bx1 = b->x1, by1 = b->y1, bx2 = b->x2, by2 = b->y2;
    if (dis(ax, ay, bx1, by1) <= ar || dis(ax, ay, bx2, by2) <= ar) return true;
    // p  is the closest point on the line to the circle center
    float u = ((ax - bx1) * (bx2 - bx1) + (ay - by1) * (by2 - by1)) / ((bx1 - bx2) * (bx1 - bx2) + (by1 - by2) * (by1 - by2));
    float x = bx1 + u * (bx2 - bx1);
    float y = by1 + u * (by2 - by1);
    // point p is on line
    PointCollideShape p(x, y);
    if (!checkCollisionLP(b, &p)) {
        return false;
    } else {
        // point p is in circle
        return checkCollisionCP(a, &p);
    }
}

bool checkCollisionLL(LineCollideShape *a, LineCollideShape *b) {
    float ax1 = a->x1, ay1 = a->y1, ax2 = a->x2, ay2 = a->y2;
    float bx1 = b->x1, by1 = b->y1, bx2 = b->x2, by2 = b->y2;
    // calculate the denominator
    float den = ((by2 - by1) * (ax2 - ax1) - (bx2 - bx1) * (ay2 - ay1));
    // denominator = 0
    if (den == 0) return false;
    float ua = ((bx2 - bx1) * (ay1 - by1) -
                (by2 - by1) * (ax1 - bx1)) / den;
    float ub = ((ax2 - ax1) * (ay1 - by1) -
                (ay2 - ay1) * (ax1 - bx1)) / den;
    // Let 's see if uA and uB tell us the lines are colliding
    return (ua >= 0 && ua <= 1) && (ub >= 0 && ub <= 1);
}

bool checkCollisionLP(LineCollideShape *a, PointCollideShape *b) {
    float ax1 = a->x1, ay1 = a->y1, ax2 = a->x2, ay2 = a->y2;
    float bx = b->getRealX(), by = b->getRealY();
    float epsilon = 0.0001;
    // length of line a
    float length = dis(ax1, ay1, ax2, ay2);
    // dis between line a point1 and point b
    float dis_a1_b = dis(ax1, ay1, bx, by);
    // dis between line a point2 and point b
    float dis_a2_b = dis(ax2, ay2, bx, by);
    // triangle law
    return std::abs(dis_a1_b + dis_a2_b - length) <= epsilon;
}
bool checkCollisionPP(PointCollideShape *a, PointCollideShape *b) {
    float epsilon = 0.0001;
    // two points
    float ax = a->getRealX(), ay = a->getRealY();
    float bx = b->getRealX(), by = b->getRealY();
    return std::abs(ax - bx) <= epsilon && std::abs(ay - by) <= epsilon;
}