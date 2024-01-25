#ifndef GAME_H_
#define GAME_H_
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "object/entity/player.h"
#include "misc/camera.h"
#include "scene/scene.h"
#include "gui/gui.h"


class Game {
    public:
        // init game
        static void init(SDL_Renderer *renderer, SDL_Window *window, int width, int height);
        static void destroy();
        static void handleInput();
        static void update(float dt);
        static void render();
        static bool isRunning();
        static void setPlayer(Player *player);
        static void setScene(Scene *scene);
        static void openGUI(GUI* gui);
        static void closeGUI(GUI* gui);

        // add shape to collision detection
        static void addCollideShape(CollideShape *shape, Object *object);
        static SDL_Window* getWindow();
        static SDL_Renderer* getRenderer();
        static Camera &getCamera();
        static Player *getPlayer();
        static Scene *getScene();
        static int getWidth();
        static int getHeight();

        // return object attach by shape
        static Object* getObjectByShape(CollideShape *shape);

        // get all shape collided with this shape
        static void getCollided(CollideShape *shape, std::vector<CollideShape*> &vec);
    private:
        inline static bool already_init;
        inline static bool running;
        // window width and height
        inline static int width, height;
        inline static SDL_Window *window;
        inline static SDL_Renderer *renderer;
        inline static std::vector<CollideShape*> shapes;
        inline static std::unordered_map<CollideShape*, Object*> shapeToObject;
        inline static Camera *camera;
        inline static Player *currentPlayer = nullptr;
        inline static Scene *scene;
        inline static std::vector<GUI*> guiStack;
};

#endif