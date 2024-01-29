#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <SDL2/SDL.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <queue>

#include "resource/sprite.h"
#include "resource/resource.h"

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

        // start worker thread, cant only run once
        static void startWorkerThread();
    private:
        static void pushToWorker(void* res);
        
        // used by async load
        inline static std::queue<void*> workQueue;
        inline static SDL_mutex* qmutex = nullptr;
        inline static SDL_mutex* wmutex = nullptr;
        inline static SDL_cond* wakeup = nullptr;
};


#endif