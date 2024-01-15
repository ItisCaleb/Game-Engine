#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL2/SDL.h>

class InputManager {
public:
    enum Key {
        W = 1,  
        A = 2,  
        S = 4,  
        D = 8,  
        SPACE = 16 
    };

    InputManager();
    ~InputManager();
    void update();  // 更新按鍵狀態
    bool isKeyPressed(Key key);  // 檢查特定按鍵是否被按下

private:
    long long keyState;  // 用於儲存按鍵狀態的整數
};

#endif
