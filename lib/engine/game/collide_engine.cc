#include "engine/collide_engine.h"

#include "engine/geomatry.h"
#include "engine/game.h"

#include <set>

void CollideEngine::addCollideShape(CollideShape *shape){
    auto obj = shape->getObject();
    if(!obj) return;
    this->shapes.push_back(shape);
    int props = obj->getProps();
    if(props & ObjectProperty::RIGID){
        this->rigidShapes.push_back(shape);
    }
    if(props & ObjectProperty::TRIGGER){
        this->triggerShapes.push_back(shape);
    }

}

void CollideEngine::getCollided(CollideShape *shape, std::vector<CollideShape*> &vec){
    for(auto s: shapes){
        if(shape == s) continue;
        if(shape->isCollide(s)) vec.push_back(s);
    }
}

void CollideEngine::searchCollides(){
    for (int i=0;i<this->rigidShapes.size();i++) {
        for(int j=i+1;j<this->rigidShapes.size();j++){
        }
    }
}

void bbSolver(BoxCollideShape *a, BoxCollideShape *b){
    if(!a->isCollide(b)) return;
    auto m = minowskiDifference(a,b);
    auto penV = penetrationVector(&m);
    Object *o = a->getObject();
    o->setX(o->getX()+penV.x);
    o->setY(o->getY()+penV.y);
}

void CollideEngine::handle(float dt){
        
    std::set<Object*> triggered;
    for (auto r1 : this->triggerShapes) {
        for (auto r2 : this->shapes) {
            if(r1 == r2) continue;
            if(!r1->isCollide(r2)) continue;
            auto obj = r1->getObject();

            // if not triggered
            if(!triggered.count(obj)){
                obj->onTrigger(r2);
            }
            triggered.insert(obj);
        }
    }

    for (auto r1 : this->rigidShapes) {
        if(r1->type != ShapeType::Box) continue;
        for (auto r2 : this->rigidShapes) {
            if(r1 == r2) continue;
            if(r2->type != ShapeType::Box) continue;
            auto obj1 = r1->getObject();
            auto obj2 = r2->getObject();
            int isStatic1 = obj1->getProps() & ObjectProperty::STATIC;
            int isStatic2 = obj2->getProps() & ObjectProperty::STATIC;
            obj1->onCollide(r2);
            obj2->onCollide(r1);
            if(!isStatic1 && isStatic2){
                bbSolver((BoxCollideShape*)r1,(BoxCollideShape*)r2);
            }else if(isStatic1 && !isStatic2){
                bbSolver((BoxCollideShape*)r2,(BoxCollideShape*)r1);
            }
        }
    }
}

void CollideEngine::drawShapes(SDL_Renderer *renderer){
    for (auto s : this->shapes) {
        s->render(renderer);
    }
}