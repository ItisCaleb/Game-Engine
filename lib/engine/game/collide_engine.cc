#include "engine/collide_engine.h"

#include "engine/geomatry.h"
#include "engine/game.h"

#include <set>

void CollideEngine::addCollideShape(CollideShape *shape){
    this->tree.insert(shape);
    this->shapes.insert(shape);
}

void CollideEngine::removeCollideShape(CollideShape *shape){
    this->tree.erase(shape);
    this->shapes.erase(shape);
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
    //shapes.clear();
    //tree.getShapes(shapes);
    for (auto r1 : shapes) {
        auto obj1 = r1->getObject();
        auto props1 = obj1->getProps();
        if(!(props1 & ObjectProperty::TRIGGER) && (props1 & ObjectProperty::NO_ONCOLLIDE))
            continue;
        collides.clear();
        tree.query(r1, collides);

        for (auto r2 : collides) {
            if(r1 == r2) continue;
            total++;
            auto obj2 = r2->getObject();
            if(collided.count(obj1)) continue;

            if(!r1->isCollide(r2)) continue;

            // trigger
            if(props1 & ObjectProperty::TRIGGER){
                obj1->onTrigger(r2);
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

            collided.insert(obj1);
        }
    }
    tree.cleanup();
    //printf("total shape:%d, compare count:%d\n",shapes.size(),total);
}

void CollideEngine::drawShapes(SDL_Renderer *renderer){
    //tree.drawGrid(renderer);
    for(auto shape: shapes){
        shape->render(renderer);
    }
}