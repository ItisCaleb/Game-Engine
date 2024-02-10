#include "engine/collide_engine.h"

#include "engine/geomatry.h"
#include "engine/game.h"


void CollideEngine::addCollideShape(CollideShape *shape){
    this->tree.insert(shape);
}

void CollideEngine::removeCollideShape(CollideShape *shape){
    this->tree.erase(shape);
}




void bbSolver(BoxCollideShape *a, BoxCollideShape *b){
    if(!a->isCollide(b)) return;
    auto m = minowskiDifference(a,b);
    auto penV = penetrationVector(&m);
    Object *o = a->getObject();
    if(penV.x != 0){
        o->setX(o->getX()+penV.x);
        o->setVelocityX(0);
    }
    if(penV.y != 0){
        o->setY(o->getY()+penV.y);
        o->setVelocityY(0);
    }
}



void CollideEngine::handle(float dt){
    std::set<Object*> collided;
    int total = 0;
    auto shapes = this->tree.getAllShape();

    for (auto r1 : shapes) {
        auto obj1 = r1->getObject();
        auto props1 = obj1->getProps();
        if(!(props1 & ObjectProperty::TRIGGER) && (props1 & ObjectProperty::NO_ONCOLLIDE))
           continue;
        triggered.clear();

        // query all collides
        tree.query(r1, collides);
        for (auto r2 : collides) {
            if(r1 == r2) continue;
            auto obj2 = r2->getObject();
            if(obj1 == obj2) continue;
            if(collided.count(obj1)) continue;

            if(!r1->isCollide(r2)) continue;
            // trigger
            if(props1 & ObjectProperty::TRIGGER){
                if(!triggered.count(obj2)){
                    triggered.insert(obj2);
                    TriggerKey key = {obj1, obj2};
                    if(!triggers.count(key)){
                        obj1->onTriggerEnter(obj2);
                        triggers.insert(std::make_pair(key, true));
                    }else{
                        obj1->onTriggerStay(obj2);
                        triggers[key] = true;
                    }
                }
            }
            

            // rigid body
            if((props1 & ObjectProperty::RIGID) &&
                (obj2->getProps() & ObjectProperty::RIGID)){
                int isStatic1 = props1 & ObjectProperty::STATIC;
                int isStatic2 = obj2->getProps() & ObjectProperty::STATIC;
                obj1->onCollide(r2);
                if(r1->type == ShapeType::Box && r2->type == ShapeType::Box){
                    if(!isStatic1 && isStatic2){
                        bbSolver((BoxCollideShape*)r1,(BoxCollideShape*)r2);
                    }else if(isStatic1 && !isStatic2){
                        bbSolver((BoxCollideShape*)r2,(BoxCollideShape*)r1);
                    }
                }
            }

        }
    }
    for(auto &it: triggers){
        if(it.second == false){
            auto key = it.first;
            key.a->onTriggerExit(key.b);
            triggers.erase(key);
            continue;
        }
        it.second = false;
    }
    //tree.cleanup();
}

void CollideEngine::drawShapes(SDL_Renderer *renderer){
    tree.cleanup();
    //tree.drawGrid(renderer);
    auto shapes = this->tree.getAllShape();
    for(auto shape: shapes){
        shape->render(renderer);
    }
}