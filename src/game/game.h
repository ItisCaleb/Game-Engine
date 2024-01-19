#ifndef GAME_H_
#define GAME_H_
#include <SDL2/SDL.h>
#include <vector>
#include <string>   
#include "entity/entity.h"
#include "misc/camera.h"

class Player;
class Game {
    public:
        static void init(SDL_Renderer *renderer, SDL_Window *window, int width, int height);
        static void destroy();
        static void update(float dt);
        static void render();
        static SDL_Window* getWindow();
        static SDL_Renderer* getRenderer();
        static Camera &getCamera();
        static Player *getPlayer();
        static void setPlayer(Player *player);
        static void setBackground(const std::string &backgroundPath);
        static void renderBackground(SDL_Renderer *renderer);

       private:
        inline static bool already_init;
        inline static SDL_Window *window;
        inline static SDL_Renderer *renderer;
        inline static std::vector<Entity*> entities;
        inline static Camera camera;
        inline static Player *currentPlayer = nullptr;
        inline static SDL_Texture *backgroundTexture = nullptr;
};

#endif