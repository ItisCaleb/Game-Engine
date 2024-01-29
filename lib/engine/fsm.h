#ifndef FSM_H_
#define FSM_H_


template <class T> class FSM{
    public:
        virtual void enter(T *instance) = 0;
        virtual FSM<T>* update(T *instance, float dt) = 0;
        virtual void exit(T *instance) = 0;
};


#endif