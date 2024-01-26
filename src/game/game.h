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
        static void init(SDL_Renderer *renderer, SDL_Window *window,
            int windowWidth, int windowHeight, int width, int height);
        static void destroy();
        static void handleInput();
        static void update(float dt);
        static void render();
        static void setPlayer(Player *player);
        static void setScene(Scene *scene);
        static void openGUI(GUI* gui);
        static void closeGUI(GUI* gui);

        // add shape to collision detection
        static void addCollideShape(CollideShape *shape, Object *object);

        // return object attach by shape
        static Object* getObjectByShape(CollideShape *shape);

        // get all shape collided with this shape
        static void getCollided(CollideShape *shape, std::vector<CollideShape*> &vec);

        // getters
        inline static bool isRunning(){
            return running;
        }

        inline static SDL_Window* getWindow(){
            return window;
        }
        inline static SDL_Renderer* getRenderer(){
            return renderer;
        }
        inline static Camera *getCamera(){
            return camera;
        }
        inline static Player *getPlayer(){
            return currentPlayer;
        }
        inline static Scene *getScene(){
            return scene;
        }

        // this is for logical width, to get window width, use Game::getWindowWidth()
        inline static int getWidth(){
            return logicWidth;
        }
        // this is for logical height, to get window width, use Game::getWindowHeight()
        inline static int getHeight(){
            return logicHeight;
        }

        // get window width
        inline static int getWindowWidth(){
            return windowWidth;
        }

        // get window height
        inline static int getWindowHeight(){
            return windowHeight;
        }

        inline static float getRenderScaleX(){
            return (float)logicWidth / windowWidth;
        }

        inline static float getRenderScaleY(){
            return (float)logicHeight / windowHeight;
        }


    private:
        inline static bool already_init;
        inline static bool running;
        // window width and height
        inline static int windowWidth, windowHeight;
        // game base resolution
        inline static int logicWidth, logicHeight;
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