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

class Scene{
    public:
        Scene(int width, int height)
            :width(width), height(height), quadTree(width, height, 8, 4), collideEngine(quadTree){}
        virtual ~Scene();
        virtual void init() = 0;
        virtual void update(float dt);
        virtual void render(SDL_Renderer *renderer);
        virtual void destroy() = 0;
    
        //get this scene width and height.
        int getWidth(){
            return width;
        }
        int getHeight(){
            return height;
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
        void renderBackground(SDL_Renderer *renderer);
        void loadScene(std::string path);
        void updatePosition(float dt);

};



#endif