#ifndef TIMER_H_
#define TIMER_H_

class Timer{
    public:
        Timer():startTicks(0),pauseTicks(0),paused(false){};
        void start();
        void restart();
        void pause();
        void unpause();
        int getTicks();
        int getTicksAndRestart();
    private:
        int startTicks;
        int pauseTicks;
        bool paused;
};

#endif