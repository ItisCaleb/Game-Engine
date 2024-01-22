#include "scene/scene.h"

#include "game/game.h"
#include "utils/resource_manager.h"

void Scene::renderBackground(SDL_Renderer* renderer){
    if(!this->background) return;

    int bgWidth, bgHeight;
    Camera &camera = Game::getCamera();
    SDL_QueryTexture(this->background, nullptr, nullptr, &bgWidth, &bgHeight);
    // set source rect to cover the whole background
    SDL_Rect srcRect;
    srcRect.w = Game::getWidth();
    srcRect.h = Game::getHeight();
    srcRect.x = (int)camera.getX();
    srcRect.y = (int)camera.getY();


    //check if source rect is out of bounds
    srcRect.x = std::max(0, std::min(srcRect.x, bgWidth - srcRect.w));
    srcRect.y = std::max(0, std::min(srcRect.y, bgHeight - srcRect.h));
    // set destination rect to cover the whole screen
    SDL_Rect destRect = {0, 0, srcRect.w, srcRect.h};
    // apply camera
    destRect = camera.apply(destRect);
    // render
    SDL_RenderCopy(renderer, this->background, &srcRect, &destRect);
}

Scene::Scene(int width, int height)
:width(width), height(height){}

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

int Scene::getWidth(){
    return this->width;
}

int Scene::getHeight(){
    return this->height;
}