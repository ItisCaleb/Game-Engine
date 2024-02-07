#include "engine/resource_manager.h"

#include <SDL2/SDL_image.h>

#include <filesystem>
#include <fstream>

#include "engine/game.h"
#include "engine/font.h"

static SDL_Texture* loadTexture(std::string resource){
    SDL_Surface* surface = IMG_Load(resource.c_str());
    if (!surface) {
        printf("Error: Unable to load surface from path: %s. SDL_image Error: %s\n", resource.c_str(), IMG_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::getRenderer(), surface);
    if (!texture) {
        printf("Error: Unable to create texture from: %s. SDL Error %s\n", resource.c_str(), SDL_GetError());
        return nullptr;
    }
    SDL_FreeSurface(surface);
    return texture;
}

// resource load functions

template <>
Sprite* ResourceManager::load(std::string resource) {
    std::filesystem::path resPath = resource;
    if (resPath.extension() == ".png" ||
        resPath.extension() == ".jpg" ||
        resPath.extension() == ".bmp") {

        // search pool first
        // we are going to store texture instead
        auto texture = (SDL_Texture*)searchPool(resource);
        if(!texture) {
            texture = loadTexture(resource);
            if(!texture) return nullptr;
            addToPool(resource, texture);
        }
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        return new Sprite(texture, 0, 0, w, h);
    } else {
        printf("Error: Unsupported format: \"%ls\". Can't load sprite from %s",
               resPath.extension().c_str(),
               resource.c_str());
        return nullptr;
    }
}

template <> 
void ResourceManager::destroy(Sprite *resource){
    int ref = ResourceManager::removeFromPool(resource->getTexture());
    if(ref == 0){
        SDL_DestroyTexture(resource->getTexture());
    }
    delete resource;
}

template <>
nlohmann::json* ResourceManager::load(std::string resource) {
    // search pool first
    auto res = (nlohmann::json*)searchPool(resource);
    if(res) return res;

    std::ifstream f(resource);
    res = new nlohmann::json(nlohmann::json::parse(f));
    addToPool(resource, res);
    return res;
}

template <> 
void ResourceManager::destroy(nlohmann::json *resource){
    int ref = ResourceManager::removeFromPool(resource);
    if(ref == 0){
        delete resource;
    }
}

template <>
Font* ResourceManager::load(std::string resource) {
    std::filesystem::path resPath = resource;
    if (resPath.extension() == ".ttf") {
        auto res = (Font*)searchPool(resource);
        int fontPt = 28;
        TTF_Font* font = TTF_OpenFont(resource.c_str(), fontPt);
        if (!font) {
            printf("Error: Unable to load surface from path: %s. SDL_ttf Error: %s\n", resource.c_str(), TTF_GetError());
            return nullptr;
        }
        res = new Font(font, fontPt);
        addToPool(resource, res);
        return res;
    } else {
        printf("Error: Unsupported format: \"%ls\". Can't load font from %s",
               resPath.extension().c_str(),
               resource.c_str());
        return nullptr;
    }
}

template <> 
void ResourceManager::destroy(Font *resource){
    int ref = ResourceManager::removeFromPool(resource);
    if(ref == 0){
        TTF_CloseFont(resource->getFont());
        delete resource;
    }
}

int ResourceManager::loadSprites(std::string resource, int clipW, int clipH, int paddingX, int paddingY, std::vector<Sprite*> &vec) {
    // search pool first
    // we are going to store texture instead
    auto texture = (SDL_Texture*)searchPool(resource);
    if(!texture) {
        texture = loadTexture(resource);
        if(!texture) return 0;
        addToPool(resource, texture);
    }
    
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

    int cnt = 0;
    for (int i = 0; i < w / (clipW+paddingX); i++) {
        for (int j = 0; j < h / (clipH+paddingY); j++) {
            int x = clipW * i;
            int y = clipH * j;
            if(i!=0) x += paddingX*i;
            if(j!=0) y += paddingY*j;
            vec.push_back(new Sprite(texture, x, y, clipW, clipH));
            cnt++;
        }
    }
    return cnt;
}

int ResourceManager::loadSprites(std::string resource, std::vector<Sprite*> &vec){
    // search pool first
    // we are going to store texture instead
    auto texture = (SDL_Texture*)searchPool(resource);
    if(!texture) {
        texture = loadTexture(resource);
        if(!texture) return 0;
        addToPool(resource, texture);
    }

    std::filesystem::path resPath = resource;
    auto* config = ResourceManager::load<nlohmann::json>(resPath.replace_extension(".json").string());
    if(!config) return 0;
    int cnt;
    for (auto& element : *config) {
        if(element["type"] == "box"){
            float x1 = element["x1"], y1 = element["y1"];
            float x2 = element["x2"], y2 = element["y2"];
            vec.push_back(new Sprite(texture, x1, y1, x2-x1, y2-y1));
        }
    }
    delete config;
    return cnt;
}

void ResourceManager::destroySprites(std::vector<Sprite*> &vec){
    SDL_Texture *last = nullptr;
    for(auto s: vec){
        if(!last || last != s->getTexture()){
            last = s->getTexture();
            ResourceManager::destroy(s);
        }
        delete s;
    }
    vec.clear();
}

// async resource load functions

template <>
AsyncResource<Sprite>* ResourceManager::loadAsync(std::string resource) {
    auto res = new AsyncResource<Sprite>(resource, ResourceType::Sprite);
    pushToWorker(res);
    return res;
}

template <>
AsyncResource<nlohmann::json>* ResourceManager::loadAsync(std::string resource) {
    auto res = new AsyncResource<nlohmann::json>(resource, ResourceType::JSON);
    pushToWorker(res);
    return res;
}

template <>
AsyncResource<TTF_Font>* ResourceManager::loadAsync(std::string resource) {
    auto res = new AsyncResource<TTF_Font>(resource, ResourceType::Font);
    pushToWorker(res);
    return res;
}