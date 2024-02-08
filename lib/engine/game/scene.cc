#include "engine/scene.h"

#include <algorithm>
#include "engine/game.h"
#include "engine/resource_manager.h"

void Scene::renderBackground(SDL_Renderer* renderer){
    if(!this->background) return;

    Camera *camera = Game::getCamera();
    // set source rect to cover the whole background
    SDL_Rect srcRect;
    srcRect.w = Game::getWidth();
    srcRect.h = Game::getHeight();
    srcRect.x = (int)camera->getX();
    srcRect.y = (int)camera->getY();

    //check if source rect is out of bounds
    srcRect.x = std::max(0, std::min(srcRect.x, this->background->getWidth() - srcRect.w));
    srcRect.y = std::max(0, std::min(srcRect.y, this->background->getHeight() - srcRect.h));
    // set destination rect to cover the whole screen
    SDL_FRect destRect = {0, 0, static_cast<float>(srcRect.w), static_cast<float>(srcRect.h)};
    // apply camera
    destRect = camera->apply(destRect);
    // render
    SDL_RenderCopyF(renderer, this->background->getTexture(), &srcRect, &destRect);
}

void Scene::updatePosition(float dt){
    for(auto o: this->objects){
        float vx = o->getVelocityX();
        float vy = o->getVelocityY();
        if(vx != 0 || vy != 0){
            for(auto s : o->hitboxs){
                this->collideEngine.removeCollideShape(s);
            }
            o->setX(o->getX() + vx*dt);
            o->setY(o->getY() + vy*dt);
            for(auto s : o->hitboxs){
                this->collideEngine.addCollideShape(s);
            }
        }
    }
}

void Scene::update(float dt){
    for (auto o : this->objects) {
        o->update(dt);
    }
    this->updatePosition(dt);
    this->collideEngine.handle(dt);

}

void Scene::render(SDL_Renderer* renderer){
    this->renderBackground(renderer);
    for (auto o : this->objects) {
        o->render(renderer);
    }
}

Scene::~Scene(){
    for (auto o : this->objects) {
        delete o;
    }
}


void Scene::loadScene(std::string path){
    /*auto j = ResourceManager::load<nlohmann::json>(path);
    for (auto& element : *j) {
        if(element["type"] == "box"){
            float x1 = element["x1"], y1 = element["y1"];
            float x2 = element["x2"], y2 = element["y2"];
            Game::addCollideShape(new BoxCollideShape(x1,y1,x2,y2));
        }else if(element["type"] == "circle"){
            float x = element["x"], y = element["y"];
            float r = element["r"];
            Game::addCollideShape(new CircleCollideShape(x,y,r));
        }else if(element["type"] == "line"){
            float x1 = element["x1"], y1 = element["y1"];
            float x2 = element["x2"], y2 = element["y2"];
            Game::addCollideShape(new LineCollideShape(x1,y1,x2,y2));
        }
    }
    delete j;*/
}

void Scene::addObject(Object *object){
    if(objects.count(object)) return;
    objects.insert(object);
    tagToObject.insert(std::make_pair(object->getTag(), object));
    for(auto s: object->hitboxs){
        this->collideEngine.addCollideShape(s);
    }
}

void Scene::removeObject(Object *object){
    objects.erase(object);
    auto iter = tagToObject.equal_range(object->getTag());
    for (auto it = iter.first; it != iter.second; ++it) {
        if (it->second == object) { 
            tagToObject.erase(it);
            break;
        }
    }
    for(auto s: object->hitboxs){
        this->collideEngine.removeCollideShape(s);
    }
    delete object;
}



Object* Scene::getObjectByTag(std::string tag){
    int nums = tagToObject.count(tag);
    if(!nums) return nullptr;
    return tagToObject.find(tag)->second;
}

void Scene::getObjectsByTag(std::string tag, std::vector<Object*> &vec){
    int nums = tagToObject.count(tag);
    if(!nums) return;
    auto iter = tagToObject.find(tag);
    while (nums--){
        vec.push_back(iter->second);
    }
  
}