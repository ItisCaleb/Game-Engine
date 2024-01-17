#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <SDL2/SDL.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <queue>

#include "misc/sprite.h"
#include "misc/resource.h"

class ResourceManager {
    public:
        // worker function
        friend int asyncIOWorker(void *data);

        // synchronous load
        template <class T> static T* load(std::string resource);

        // asynchronous load
        template <class T> static AsyncResource<T>* loadAsync(std::string resource);
        static std::vector<Sprite*>* loadSprites(std::string resource, int clipW, int clipH);

        // start worker thread, cant only run once
        static void startWorkerThread();
    private:
        static void pushToWorker(void* res);
        
        // used by async load
        inline static std::queue<void*> workQueue;
        inline static SDL_mutex* mutex = nullptr;
};


#endif