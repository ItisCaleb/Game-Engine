#include "utils/resource_manager.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <filesystem>
#include <fstream>

#include "game/game.h"

// resource load functions
template <>
SDL_Texture* ResourceManager::load(std::string resource) {
    std::filesystem::path resPath = resource;
    if (resPath.extension() == ".png" ||
        resPath.extension() == ".jpg" ||
        resPath.extension() == ".bmp") {
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
    } else {
        printf("Error: Unsupported format: \"%s\". Can't load sprite from %s",
               resPath.extension().c_str(),
               resource.c_str());
        return nullptr;
    }
}

template <>
Sprite* ResourceManager::load(std::string resource) {
    auto* texture = ResourceManager::load<SDL_Texture>(resource);
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    return new Sprite(texture, 0, 0, w, h);
}

template <>
nlohmann::json* ResourceManager::load(std::string resource) {
    std::ifstream f(resource);
    return new nlohmann::json(nlohmann::json::parse(f));
}

template <>
TTF_Font* ResourceManager::load(std::string resource) {
    std::filesystem::path resPath = resource;
    if (resPath.extension() == ".ttf") {
        TTF_Font* font = TTF_OpenFont(resource.c_str(), 28);
        if (!font) {
            printf("Error: Unable to load surface from path: %s. SDL_ttf Error: %s\n", resource.c_str(), TTF_GetError());
            return nullptr;
        }
        return font;
    } else {
        printf("Error: Unsupported format: \"%s\". Can't load font from %s",
               resPath.extension().c_str(),
               resource.c_str());
        return nullptr;
    }
}

std::vector<Sprite*>* ResourceManager::loadSprites(std::string resource, int clipW, int clipH) {
    std::filesystem::path resPath = resource;
    auto* texture = ResourceManager::load<SDL_Texture>(resource);
    int w, h;
    auto* sprites = new std::vector<Sprite*>;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    for (int i = 0; i < w / clipW; i++) {
        for (int j = 0; j < h / clipH; j++) {
            sprites->push_back(new Sprite(texture, clipW * i, clipH * j, clipW, clipH));
        }
    }
    return sprites;
}


inline void ResourceManager::pushToWorker(void* res){
    SDL_LockMutex(ResourceManager::qmutex);
    workQueue.push(res);
    SDL_UnlockMutex(ResourceManager::qmutex);
    SDL_CondSignal(ResourceManager::wakeup);
}

// async resource load functions
template <>
AsyncResource<SDL_Texture>* ResourceManager::loadAsync(std::string resource) {
    auto res = new AsyncResource<SDL_Texture>(resource, ResourceType::Texture);
    pushToWorker(res);
    return res;
}

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

// worker function to load resource
int asyncIOWorker(void* data) {
    while (true) {
        SDL_LockMutex(ResourceManager::wmutex);
        SDL_CondWait(ResourceManager::wakeup, ResourceManager::wmutex);
        SDL_UnlockMutex(ResourceManager::wmutex);
        // if no request in queue, then continue
        while (!ResourceManager::workQueue.empty()){

            // queue isn't thread-safe, so we need to lock
            SDL_LockMutex(ResourceManager::qmutex);
            auto res = (AsyncResource<void>*)ResourceManager::workQueue.front();
            ResourceManager::workQueue.pop();
            SDL_UnlockMutex(ResourceManager::qmutex);
    
            // load resource based on type
            switch (res->type) {
                case ResourceType::Texture:
                    res->resource = ResourceManager::load<SDL_Texture>(res->getPath());
                    break;
                case ResourceType::Sprite:
                    res->resource = ResourceManager::load<Sprite>(res->getPath());
                    break;
                case ResourceType::JSON:
                    res->resource = ResourceManager::load<nlohmann::json>(res->getPath());
                    break;
                case ResourceType::Font:
                    res->resource = ResourceManager::load<TTF_Font>(res->getPath());
                    break;
                default:
                    res->resource = nullptr;
                    break;
            }
            res->available = true;
        }
    }
    
    return 0;
}

void ResourceManager::startWorkerThread() {
    if (qmutex) return;
    qmutex = SDL_CreateMutex();
    wmutex = SDL_CreateMutex();
    wakeup = SDL_CreateCond();
    SDL_Thread* thread = SDL_CreateThread(asyncIOWorker, "Resource Worker", nullptr);
    if (!thread) {
        printf("Unable to create thread: %s\n", SDL_GetError());
        return;
    }
}

