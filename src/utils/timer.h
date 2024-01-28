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
        const inline static int TICK_60FRAMES = 17;
        const inline static int TICK_30FRAMES = 33;
        const inline static int TICK_24FRAMES = 42;
        const inline static int TICK_12FRAMES = 83;
    private:
        int startTicks;
        int pauseTicks;
        bool paused;
};

#endif