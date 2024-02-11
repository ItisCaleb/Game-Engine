#ifndef FSM_H_
#define FSM_H_


template <class T> class FSM{
    public:
        virtual ~FSM<T>(){}
        virtual void enter(T *instance) = 0;
        virtual FSM<T>* update(T *instance, float dt) = 0;
        virtual void exit(T *instance) = 0;
};

template <class T> 
class FSMController{
    private:
        FSM<T> *state = nullptr;
        FSM<T> *force_state = nullptr;
    public:
        void init(FSM<T> *state, T* instance){
            this->state = state;
            this->state->enter(instance);
        }
        void update(T* instance, float dt){
            if(!this->state) return;
            auto _state = this->state->update(instance, dt);
            // if force
            if(force_state){
                if(_state) delete _state;
                _state = force_state;
                force_state = nullptr;
            }
            // normal
            if(_state){
                this->state->exit(instance);
                delete this->state;
                this->state = _state;
                this->state->enter(instance);
            }
        }
        void force(FSM<T> *state){
            this->force_state = state;
        }
};


#endif