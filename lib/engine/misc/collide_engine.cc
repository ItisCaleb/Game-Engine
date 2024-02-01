#include "engine/collide_engine.h"

#include <cmath>

void CollideEngine::addCollideShape(CollideShape *shape){
    auto obj = shape->getObject();
    if(!obj) return;
    this->shapes.push_back(shape);
    if(obj->isRigid()){
        this->rigidShapes.push_back(shape);
    }

}

void CollideEngine::getCollided(CollideShape *shape, std::vector<CollideShape*> &vec){
    for(auto s: shapes){
        if(shape == s) continue;
        if(shape->isCollide(s)) vec.push_back(s);
    }
}
void bbRigid(BoxCollideShape *r1, BoxCollideShape *r2){
    if(!r1->isCollide(r2)) return;
    float lambda = 0.1;
    SDL_FRect f1 = {r1->getRealX(),r1->getRealY(),r1->w,r1->h};
    SDL_FRect f2 = {r2->getRealX(),r2->getRealY(),r2->w,r2->h};
    SDL_FRect overlap;
    SDL_IntersectFRect(&f1, &f2, &overlap);
    auto o1 = r1->getObject();
    printf("%f %f\n",overlap.w,overlap.h);
    if(overlap.w > overlap.h){
        if(o1->getVelocityY()> 0){
            o1->setY(r2->getRealY() - r1->h - lambda);
        }else{
            o1->setY(r2->getRealY() + r2->h + lambda);
        }
    }else{
        if(o1->getVelocityX()> 0){
            o1->setX(r2->getRealX() - r1->w - lambda);
        }else{
            o1->setX(r2->getRealX() + r2->w + lambda);
        }
    }
}

void CollideEngine::handleRigid(float dt){
        

    for (auto r1 : this->rigidShapes) {
        if(r1->type != ShapeType::Box) continue;
        for (auto r2 : this->rigidShapes) {
            if(r2->type != ShapeType::Box) continue;
            if(!r1->getObject()->isStatic() && r2->getObject()->isStatic()){
                bbRigid((BoxCollideShape*)r1,(BoxCollideShape*)r2);
            }else if(r1->getObject()->isStatic() && !r2->getObject()->isStatic()){
                bbRigid((BoxCollideShape*)r2,(BoxCollideShape*)r1);
            }

        }
    }
}

void CollideEngine::drawShapes(SDL_Renderer *renderer){
    for (auto s : this->shapes) {
        s->render(renderer);
    }
}