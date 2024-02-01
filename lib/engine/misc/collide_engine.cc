#include "engine/collide_engine.h"

void CollideEngine::addCollideShape(CollideShape *shape){
    auto obj = shape->getObject();
    if(!obj) return;
    this->shapes.push_back(shape);
    if(obj->getProps() & ObjectProperty::RIGID){
        this->rigidShapes.push_back(shape);
    }

}

void CollideEngine::getCollided(CollideShape *shape, std::vector<CollideShape*> &vec){
    for(auto s: shapes){
        if(shape == s) continue;
        if(shape->isCollide(s)) vec.push_back(s);
    }
}

void CollideEngine::handleRigid(){
    for (auto r1 : this->rigidShapes) {
        if(r1->type != ShapeType::Box) continue;
        for (auto r2 : this->rigidShapes) {
            if(r2->type != ShapeType::Box) continue;
            if(r1->isCollide(r2)){
                auto _r1 = (BoxCollideShape*)r1;
                auto _r2 = (BoxCollideShape*)r2;
                SDL_FRect f1 = {_r1->x,_r1->y,_r1->w,_r1->h};
                SDL_FRect f2 = {_r1->x,_r1->y,_r1->w,_r1->h};
                SDL_FRect overlap;
                SDL_IntersectFRect(&f1, &f2, &overlap);
                if(overlap.x > overlap.y){
                    
                }
            }
        }
    }
}

void CollideEngine::drawShapes(SDL_Renderer *renderer){
    for (auto s : this->shapes) {
        s->render(renderer);
    }
}