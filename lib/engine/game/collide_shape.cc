#include "engine/collide_shape.h"

#include "engine/game.h"
#include "engine/geomatry.h"

static bool checkCollisionBB(BoxCollideShape *a, BoxCollideShape *b);
static bool checkCollisionBC(BoxCollideShape *a, CircleCollideShape *b);
static bool checkCollisionBL(BoxCollideShape *a, LineCollideShape *b);
static bool checkCollisionBP(BoxCollideShape *a, PointCollideShape *b);
static bool checkCollisionCC(CircleCollideShape *a, CircleCollideShape *b);
static bool checkCollisionCP(CircleCollideShape *a, PointCollideShape *b);
static bool checkCollisionCL(CircleCollideShape *a, LineCollideShape *b);
static bool checkCollisionLL(LineCollideShape *a, LineCollideShape *b);
static bool checkCollisionLP(LineCollideShape *a, PointCollideShape *b);
static bool checkCollisionPP(PointCollideShape *a, PointCollideShape *b);



bool BoxCollideShape::isCollide(CollideShape *shape) {
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

void BoxCollideShape::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_FRect rect = {.x = this->getRealX(), .y = this->getRealY(), .w = this->w, .h = this->h};
    rect = Game::getCamera()->apply(rect);
    SDL_RenderDrawRectF(renderer, &rect);
}

bool CircleCollideShape::isCollide(CollideShape *shape) {
    switch (shape->type) {
        case ShapeType::Box:
            return checkCollisionBC(reinterpret_cast<BoxCollideShape *>(shape), this);
        case ShapeType::Circle:
            return checkCollisionCC(this, reinterpret_cast<CircleCollideShape *>(shape));
        case ShapeType::Line:
            return checkCollisionCL(this, reinterpret_cast<LineCollideShape *>(shape));
        case ShapeType::Point:
            return checkCollisionCP(this, reinterpret_cast<PointCollideShape *>(shape));
        default:
            return false;
    }
}

void CircleCollideShape::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
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
        SDL_RenderDrawPointF(renderer, ax + x, ay - y);
        SDL_RenderDrawPointF(renderer, ax + x, ay + y);
        SDL_RenderDrawPointF(renderer, ax - x, ay - y);
        SDL_RenderDrawPointF(renderer, ax - x, ay + y);
        SDL_RenderDrawPointF(renderer, ax + y, ay - x);
        SDL_RenderDrawPointF(renderer, ax + y, ay + x);
        SDL_RenderDrawPointF(renderer, ax - y, ay - x);
        SDL_RenderDrawPointF(renderer, ax - y, ay + x);

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
    switch (shape->type) {
        case ShapeType::Box:
            return checkCollisionBL(reinterpret_cast<BoxCollideShape *>(shape), this);
        case ShapeType::Circle:
            return checkCollisionCL(reinterpret_cast<CircleCollideShape *>(shape), this);
        case ShapeType::Line:
            return checkCollisionLL(this, reinterpret_cast<LineCollideShape *>(shape));
        case ShapeType::Point:
            return checkCollisionLP(this, reinterpret_cast<PointCollideShape *>(shape));
        default:
            return false;
    }
}

void LineCollideShape::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    float ax1 = Game::getCamera()->applyX(this->x1);
    float ay1 = Game::getCamera()->applyY(this->y1);
    float ax2 = Game::getCamera()->applyX(this->x2);
    float ay2 = Game::getCamera()->applyY(this->y2);
    SDL_RenderDrawLineF(renderer, ax1, ay1, ax2, ay2);
}

bool PointCollideShape::isCollide(CollideShape *shape) {
    switch (shape->type) {
        case ShapeType::Box:
            return checkCollisionBP(reinterpret_cast<BoxCollideShape *>(shape), this);
        case ShapeType::Circle:
            return checkCollisionCP(reinterpret_cast<CircleCollideShape *>(shape), this);
        case ShapeType::Line:
            return checkCollisionLP(reinterpret_cast<LineCollideShape *>(shape), this);
        case ShapeType::Point:
            return checkCollisionPP(reinterpret_cast<PointCollideShape *>(shape), this);
        default:
            return false;
    }
}

void PointCollideShape::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    float ax = Game::getCamera()->applyX(this->getRealX());
    float ay = Game::getCamera()->applyY(this->getRealY());
    SDL_RenderDrawPointF(renderer, ax, ay);
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
    bool left = checkCollisionLL(&l1, b);
    bool right = checkCollisionLL(&l2, b);
    bool top = checkCollisionLL(&l3, b);
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
    return dis(a->getRealX(), a->getRealY(), b->getRealX(), b->getRealY()) <= (a->r + b->r);
}

bool checkCollisionCP(CircleCollideShape *a, float x, float y) {
    // distance between circle and points <=  r
    return dis(a->getRealX(), a->getRealY(), x, y) <= a->r;
}
bool checkCollisionCP(CircleCollideShape *a, PointCollideShape *b) {
    // distance between circle and points <=  r
    return dis(a->getRealX(), a->getRealY(), b->getRealX(), b->getRealY()) <= a->r;
}
bool checkCollisionCL(CircleCollideShape *a, LineCollideShape *b) {
    // line points in circle
    if (dis(a->getRealX(), a->getRealY(), b->x1, b->y1) <= a->r || dis(a->getRealX(), a->getRealY(), b->x2, b->y2) <= a->r) return true;
    // p  is the closest point on the line to the circle center
    float u = ((a->getRealX() - b->x1) * (b->x2 - b->x1) + (a->getRealY() - b->y1) * (b->y2 - b->y1)) / ((b->x1 - b->x2) * (b->x1 - b->x2) + (b->y1 - b->y2) * (b->y1 - b->y2));
    float x = b->x1 + u * (b->x2 - b->x1);
    float y = b->y1 + u * (b->y2 - b->y1);
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

bool checkCollisionLP(LineCollideShape *a, PointCollideShape *b) {
    float epsilon = 0.0001;
    // length of line a
    float length = dis(a->x1, a->y1, a->x2, a->y2);
    // dis between line a point1 and point b
    float dis_a1_b = dis(a->x1, a->y1, b->getRealX(), b->getRealY());
    // dis between line a point2 and point b
    float dis_a2_b = dis(a->x2, a->y2, b->getRealX(), b->getRealY());
    // triangle law
    return std::abs(dis_a1_b + dis_a2_b - length) <= epsilon;
}
bool checkCollisionPP(PointCollideShape *a, PointCollideShape *b) {
    float epsilon = 0.0001;
    // two points
    return std::abs(a->getRealX() - b->getRealX()) <= epsilon && std::abs(a->getRealY() - b->getRealY()) <= epsilon;
}