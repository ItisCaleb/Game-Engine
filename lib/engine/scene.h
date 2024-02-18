#ifndef SCENE_H_
#define SCENE_H_

#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>

#include "engine/sprite.h"
#include "engine/object.h"
#include "engine/collide_shape.h"
#include "engine/collide_engine.h"
#include "engine/quad_tree.h"
#include "engine/path_finder.h"

class Scene{
    public:
        Scene(int width)
            :width(width), quadTree(width, 8, 4),
             collideEngine(quadTree), pathFinder(quadTree, width, 16){}
        virtual ~Scene();
        virtual void init() = 0;
        virtual void update(float dt);
        virtual void render(Renderer *renderer);
        virtual void destroy() = 0;
    
        //get this scene width and height.
        int getWidth(){
            return width;
        }
        void addObject(Object *object);
        void removeObject(Object *object);

        // return object attach by tag
        Object* getObjectByTag(std::string tag);
        void getObjectsByTag(std::string tag, std::vector<Object*> &vec);
        
    protected:
        int width, height;
        Sprite *background;
        Sprite *foreground;
        std::set<Object*> objects;
        std::unordered_multimap<std::string, Object*> tagToObject;
        QuadTree quadTree;
        CollideEngine collideEngine;
        PathFinder pathFinder;
        void renderBackground(Renderer *renderer);
        void loadScene(std::string path);
        void updatePosition(float dt);

};



#endif