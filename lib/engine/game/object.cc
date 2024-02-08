#include "engine/object.h"

#include "engine/collide_shape.h"
#include "engine/game.h"

void Object::attachHitbox(CollideShape *shape){
    shape->object = this;
    this->hitboxs.push_back(shape);
}
