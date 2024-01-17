#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <string>

#include "utils/resource_manager.h"

enum class ResourceType{
    Texture, Sprite, JSON, Audio
};

template <class T> class AsyncResource{
    public:
        friend class ResourceManager;
        friend int asyncIOWorker(void *data);
        AsyncResource(std::string resource, ResourceType type)
            :path(resource),type(type),available(0){};
        std::string getPath(){
            return path;
        }
        bool isAvailabe(){
            return available;
        }
        T* getResource(){
            return resource;
        }
    private:
        std::string path;
        ResourceType type;
        bool available;
        T *resource;
};

#endif