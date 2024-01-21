#ifndef OBJECT_H_
#define OBJECT_H_


enum class ObjectType{
    Entity, Wall, Trigger
};

class Object{
    public:
        const ObjectType type;
        virtual void update(float dt) = 0;
        virtual void render(SDL_Renderer *renderer) = 0;
    protected:
        Object(ObjectType type)
            :type(type){}
};


#endif