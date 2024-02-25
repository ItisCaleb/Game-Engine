#include "engine/resource_manager.h"

#include <SDL2/SDL_image.h>

#include <filesystem>
#include <fstream>

#include "engine/game.h"
#include "engine/font.h"


void* ResourceManager::searchPool(std::string path){
    SDL_LockMutex(pool_mutex);
    // use reference counter
    auto iter = resourcePool.find(path);
    if(iter == resourcePool.end()) return nullptr;
    iter->second.second++;
    SDL_UnlockMutex(pool_mutex);
    return iter->second.first;
}

void ResourceManager::addToPool(std::string path, void *resource){
    SDL_LockMutex(pool_mutex);
    auto pair = std::pair<void*, int>(resource, 1);
    resourcePool[path] = pair;
    SDL_UnlockMutex(pool_mutex);
}

int ResourceManager::removeFromPool(void *resource){
    SDL_LockMutex(pool_mutex);
    for(auto iter = resourcePool.begin(); iter != resourcePool.end(); iter++)
        if(iter->second.first == resource){
            // ref count - 1
            iter->second.second--;
            int ref = iter->second.second;
            if(ref == 0){
                resourcePool.erase(iter);
            }
            SDL_UnlockMutex(pool_mutex);
            return ref;
        }
    SDL_UnlockMutex(pool_mutex);
    return -1;
}



void ResourceManager::pushToWorker(void* res){
    SDL_LockMutex(ResourceManager::qmutex);
    workQueue.push(res);
    SDL_UnlockMutex(ResourceManager::qmutex);
    SDL_CondSignal(ResourceManager::wakeup);
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
                case ResourceType::JSON:
                    res->resource = ResourceManager::load<nlohmann::json>(res->getPath());
                    break;
                case ResourceType::Font:
                    res->resource = ResourceManager::load<Font>(res->getPath());
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
    pool_mutex = SDL_CreateMutex();
    qmutex = SDL_CreateMutex();
    wmutex = SDL_CreateMutex();
    wakeup = SDL_CreateCond();
    SDL_Thread* thread = SDL_CreateThread(asyncIOWorker, "Resource Worker", nullptr);
    if (!thread) {
        printf("Unable to create thread: %s\n", SDL_GetError());
        return;
    }
}

