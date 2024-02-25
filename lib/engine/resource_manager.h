#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <SDL2/SDL.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <utility>

#include "engine/components/sprite.h"
#include "engine/resource.h"

class ResourceManager {
    public:
        // worker function
        friend int asyncIOWorker(void *data);

        // synchronous load
        template <class T> static T* load(std::string resource);

        // asynchronous load
        template <class T> static AsyncResource<T>* loadAsync(std::string resource);

        // load sprites
        // will return number of sprites loaded
        static int loadSprites(std::string resource, int clipW, int clipH, int paddingX, int paddingY, std::vector<Sprite*> &vec);

        // load sprites, but will retrieve every clip from JSON with same name
        // example: player.png player.json
        // will return number of sprites loaded
        static int loadSprites(std::string resource, std::vector<Sprite*> &vec);

        template <class T> static void destroy(T *resource);

        static void destroySprites(std::vector<Sprite*> &vec);

        // start worker thread, cant only run once
        static void startWorkerThread();
    private:
        static void pushToWorker(void* res);
        static void* searchPool(std::string path);
        static void addToPool(std::string path, void *resource);
        
        // This will return ref count left
        // If resource is already destroyed, then it will return -1
        static int removeFromPool(void *resource);

        inline static std::map
            <std::string,std::pair<void*, int>> resourcePool;
        inline static SDL_mutex* pool_mutex = nullptr;
        
        // used by async load
        inline static std::queue<void*> workQueue;
        inline static SDL_mutex* qmutex = nullptr;
        inline static SDL_mutex* wmutex = nullptr;
        inline static SDL_cond* wakeup = nullptr;
};


#endif