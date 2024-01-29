#include "engine/collide_shape.h"

#include "engine/game.h"

float dis(float ax, float ay, float bx, float by) {
    return std::sqrt(std::pow(ax - bx, 2) + std::pow(ay - by, 2));
}
bool checkCollisionBB(BoxCollideShape *a, BoxCollideShape *b);
bool checkCollisionBC(BoxCollideShape *a, CircleCollideShape *b);
bool checkCollisionBL(BoxCollideShape *a, LineCollideShape *b);
bool checkCollisionBP(BoxCollideShape *a, PointCollideShape *b);
bool checkCollisionCC(CircleCollideShape *a, CircleCollideShape *b);
bool checkCollisionCP(CircleCollideShape *a, PointCollideShape *b);
bool checkCollisionCL(CircleCollideShape *a, LineCollideShape *b);
bool checkCollisionLL(LineCollideShape *a, LineCollideShape *b);
bool checkCollisionLP(LineCollideShape *a, PointCollideShape *b);
bool checkCollisionPP(PointCollideShape *a, PointCollideShape *b);

bool BoxCollideShape::isCollide(CollideShape *shape) {
    switch (shape->type) {
        case ShapeType::Box:
            return checkCollisionBB(this, dynamic_cast<BoxCollideShape *>(shape));
        case ShapeType::Circle:
            return checkCollisionBC(this, dynamic_cast<CircleCollideShape *>(shape));
        case ShapeType::Line:
            return checkCollisionBL(this, dynamic_cast<LineCollideShape *>(shape));
        case ShapeType::Point:
            return checkCollisionBP(this, dynamic_cast<PointCollideShape *>(shape));
    }
}

void BoxCollideShape::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_FRect rect = {.x = this->x1, .y = this->y1, .w = this->x2 - this->x1, .h = this->y2 - this->y1};
    rect = Game::getCamera()->apply(rect);
    SDL_RenderDrawRectF(renderer, &rect);
}

bool CircleCollideShape::isCollide(CollideShape *shape) {
    switch (shape->type) {
        case ShapeType::Box:
            return checkCollisionBC(dynamic_cast<BoxCollideShape *>(shape), this);
        case ShapeType::Circle:
            return checkCollisionCC(this, dynamic_cast<CircleCollideShape *>(shape));
        case ShapeType::Line:
            return checkCollisionCL(this, dynamic_cast<LineCollideShape *>(shape));
        case ShapeType::Point:
            return checkCollisionCP(this, dynamic_cast<PointCollideShape *>(shape));
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
    float ax = Game::getCamera()->applyX(this->x);
    float ay = Game::getCamera()->applyY(this->y);
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
            return checkCollisionBL(dynamic_cast<BoxCollideShape *>(shape), this);
        case ShapeType::Circle:
            return checkCollisionCL(dynamic_cast<CircleCollideShape *>(shape), this);
        case ShapeType::Line:
            return checkCollisionLL(this, dynamic_cast<LineCollideShape *>(shape));
        case ShapeType::Point:
            return checkCollisionLP(this, dynamic_cast<PointCollideShape *>(shape));
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
            return checkCollisionBP(dynamic_cast<BoxCollideShape *>(shape), this);
        case ShapeType::Circle:
            return checkCollisionCP(dynamic_cast<CircleCollideShape *>(shape), this);
        case ShapeType::Line:
            return checkCollisionLP(dynamic_cast<LineCollideShape *>(shape), this);
        case ShapeType::Point:
            return checkCollisionPP(dynamic_cast<PointCollideShape *>(shape), this);
    }
}

void PointCollideShape::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    float ax = Game::getCamera()->applyX(this->x);
    float ay = Game::getCamera()->applyY(this->y);
    SDL_RenderDrawPointF(renderer, ax, ay);
}

bool checkCollisionBB(BoxCollideShape *a, BoxCollideShape *b) {
    // two boxes
    if (a->y2 < b->y1) return false;
    if (a->y1 > b->y2) return false;
    if (a->x2 < b->x1) return false;
    if (a->x1 > b->x2) return false;
    return true;
}
bool checkCollisionBC(BoxCollideShape *a, CircleCollideShape *b) {
    // get point in rectangle which is close to circle
    // init test = circle center (if circle center is in box)
    float tx = b->x;
    float ty = b->y;
    // if circle center is to the right of box take right side
    if (b->x < a->x1)
        tx = a->x1;
    else if (b->x > a->x2)
        tx = a->x2;
    if (b->y < a->y1)
        ty = a->y1;
    else if (b->y > a->y2)
        ty = a->y2;
    return dis(b->x, b->y, tx, ty) <= b->r;
}
bool checkCollisionBL(BoxCollideShape *a, LineCollideShape *b) {
    PointCollideShape p1(b->x1, b->y1);
    PointCollideShape p2(b->x2, b->y2);
    if (checkCollisionBP(a, &p1) ||
        checkCollisionBP(a, &p2)) {
        return true;
    }
    LineCollideShape l1(a->x1, a->y1, a->x1, a->y2);
    LineCollideShape l2(a->x2, a->y1, a->x2, a->y2);
    LineCollideShape l3(a->x1, a->y1, a->x2, a->y1);
    LineCollideShape l4(a->x1, a->y2, a->x2, a->y2);
    bool left = checkCollisionLL(&l1, b);
    bool right = checkCollisionLL(&l2, b);
    bool top = checkCollisionLL(&l3, b);
    bool bottom = checkCollisionLL(&l4, b);
    return left || right || top || bottom;
}

bool checkCollisionBP(BoxCollideShape *a, PointCollideShape *b) {
    if (a->x2 < b->x) return false;
    if (a->x1 > b->x) return false;
    if (a->y2 < b->y) return false;
    if (a->y1 > b->y) return false;
    return true;
}
bool checkCollisionCC(CircleCollideShape *a, CircleCollideShape *b) {
    // distance between two points <=  (ra + rb)
    return dis(a->x, a->y, b->x, b->y) <= (a->r + b->r);
}

bool checkCollisionCP(CircleCollideShape *a, float x, float y) {
    // distance between circle and points <=  r
    return dis(a->x, a->y, x, y) <= a->r;
}
bool checkCollisionCP(CircleCollideShape *a, PointCollideShape *b) {
    // distance between circle and points <=  r
    return dis(a->x, a->y, b->x, b->y) <= a->r;
}
bool checkCollisionCL(CircleCollideShape *a, LineCollideShape *b) {
    // line points in circle
    if (dis(a->x, a->y, b->x1, b->y1) <= a->r || dis(a->x, a->y, b->x2, b->y2) <= a->r) return true;
    // p  is the closest point on the line to the circle center
    float u = ((a->x - b->x1) * (b->x2 - b->x1) + (a->y - b->y1) * (b->y2 - b->y1)) / ((b->x1 - b->x2) * (b->x1 - b->x2) + (b->y1 - b->y2) * (b->y1 - b->y2));
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
    float dis_a1_b = dis(a->x1, a->y1, b->x, b->y);
    // dis between line a point2 and point b
    float dis_a2_b = dis(a->x2, a->y2, b->x, b->y);
    // triangle law
    return std::abs(dis_a1_b + dis_a2_b - length) <= epsilon;
}
bool checkCollisionPP(PointCollideShape *a, PointCollideShape *b) {
    float epsilon = 0.0001;
    // two points
    return std::abs(a->x - b->x) <= epsilon && std::abs(a->y - b->y) <= epsilon;
}