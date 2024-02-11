#ifndef COLLIDE_ENGINE_H_
#define COLLIDE_ENGINE_H_

#include <SDL2/SDL.h>

#include "engine/collide_shape.h"
#include "engine/quad_tree.h"
#include <vector>
#include <utility>
#include <set>
#include <unordered_map>

struct TriggerKey{
    Object *a, *b;
    std::size_t operator () (const TriggerKey &p) const {
        return ((long long)p.a ^ (((long long)p.b) << 1));
    }
    inline bool operator==(const TriggerKey &p) const {
		return a == p.a && b == p.b;
    }
};

class CollideEngine{
    public:
        CollideEngine(int w, int h)
            :tree(w,h,8, 4){}
        void handle(float dt);
        // add shape to collision detection
        void addCollideShape(CollideShape *shape);
        void removeCollideShape(CollideShape *shape);
        void drawShapes(SDL_Renderer *renderer);
    private:
        void insertTrigger(Object *a, Object *b);
        QuadTree tree;
        std::vector<CollideShape*> collides;

        // trigger
        std::unordered_map<TriggerKey, bool, TriggerKey> triggers;
        std::set<Object*> triggered;
};

#endif