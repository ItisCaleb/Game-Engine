#ifndef GAME_H_
#define GAME_H_
#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "entity/player.h"
#include "misc/camera.h"
#include "../utils/resource_manager.h"

class Game {
    public:
        static void init(SDL_Renderer *renderer, SDL_Window *window, int width, int height);
        static void destroy();
        static void update(float dt);
        static void render();
        static void setPlayer(Player *player);
        static void loadBackground(const std::string &backgroundPath);
        static void renderBackground(SDL_Renderer *renderer, const Camera &cam);
        static SDL_Window* getWindow();
        static SDL_Renderer* getRenderer();
        static Camera &getCamera();
        static Player *getPlayer();

    private:
        inline static bool already_init;
        inline static int width, height;
        inline static SDL_Window *window;
        inline static SDL_Renderer *renderer;
        inline static std::vector<Entity*> entities;
        inline static Camera camera;
        inline static Player *currentPlayer = nullptr;
        inline static SDL_Texture *backgroundTexture = nullptr;
};

#endif