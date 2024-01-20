#ifndef GAME_H_
#define GAME_H_
#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "entity/player.h"
#include "misc/camera.h"
#include "scene/scene.h"

class Game {
    public:
        static void init(SDL_Renderer *renderer, SDL_Window *window, int width, int height);
        static void destroy();
        static void update(float dt);
        static void render();
        static void setPlayer(Player *player);
        static void setScene(Scene *scene);
        static SDL_Window* getWindow();
        static SDL_Renderer* getRenderer();
        static Camera &getCamera();
        static Player *getPlayer();
        static Scene *getScene();
        static int getWidth();
        static int getHeight();

    private:
        inline static bool already_init;

        // window width and height
        inline static int width, height;
        inline static SDL_Window *window;
        inline static SDL_Renderer *renderer;
        inline static std::vector<Entity*> entities;
        inline static Camera camera;
        inline static Player *currentPlayer = nullptr;
        inline static Scene *scene;
};

#endif