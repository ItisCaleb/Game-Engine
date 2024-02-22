#include "engine/game.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <utility>

#include "engine/input_manager.h"
#include "engine/gui_helper.h"


SDL_Renderer* Game::initSDL(std::string windowName){
    // Init everything
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        //handle error
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n",
                SDL_GetError());
        exit(1);
    }

    // set hints
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    //get screen size
    // set window width and height
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    printf("screen_width: %d, screen_height: %d\n", dm.w, dm.h);
    Game::windowWidth  = dm.w;
    //-60 for the taskbar
    Game::windowHeight = dm.h-60;

    SDL_Window *window = SDL_CreateWindow(windowName.c_str(),
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          Game::windowWidth, Game::windowHeight,
                                          SDL_WINDOW_RESIZABLE);

    if (!window) {
        printf("Error: Failed to open window\nSDL Error: '%s'\n",
                SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("Error: SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        SDL_Quit();
        exit(1);
    }
    if(TTF_Init() == -1){
        printf("Error: SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        exit(1);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }
    Game::window = window;
    return renderer;
}

void Game::init(std::string windowName, int width, int height){
    if (Game::already_init) return;
    SDL_Renderer *renderer = Game::initSDL(windowName);
    SDL_RenderSetLogicalSize(renderer, width, height);
    //float scale = std::min((float)windowWidth/width, (float)windowHeight/height);
    //SDL_RenderSetScale(renderer, scale, scale);
    Game::ecs = new ECS();
    Game::camera = new Camera(width, height);
    Game::renderer = new Renderer(renderer, camera);
    Game::already_init = true;
    Game::running = true;

    Game::logicWidth = width;
    Game::logicHeight = height;


    
    GUIHelper::init();
}


void Game::setScene(Scene *scene) {
    if(Game::scene){
        delete Game::scene;
    }
    Game::scene = scene;
    scene->init();
}

void Game::openGUI(GUI* gui){
    if(!gui->isOpened()){
        guiStack.push_back(gui);
        gui->onOpen();
        gui->opened = true;
    }
}

void Game::closeGUI(GUI* gui){
    if(gui->isOpened()){
        if(guiStack.back() == gui){
            guiStack.pop_back();
            gui->onClose();
            gui->opened = false;
        }
    }
}




void Game::destroy() {
    delete Game::camera;
    delete Game::renderer;
    SDL_DestroyWindow(Game::window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void Game::handleInput(){
    SDL_Event e;
    //SDL_StartTextInput();
    while (SDL_PollEvent(&e)) {
        // handle gui input
        if(!guiStack.empty())
            GUIHelper::handleInput(e);

        // handle normal unput
        switch (e.type) {
            case SDL_QUIT:
                Game::running = false;
                break;
            
            case SDL_WINDOWEVENT:
                switch (e.window.event){
                    case SDL_WINDOWEVENT_RESIZED:
                        Game::windowWidth = e.window.data1;
                        Game::windowHeight = e.window.data2;
                        break;
                    #ifdef _WIN32
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        // disable windows ime
                        ImmAssociateContext(GetForegroundWindow(), NULL);
                        break;
                    #endif
                }
                break;
            case SDL_MOUSEWHEEL:
                InputManager::updateMouseWheelScroll(e.wheel.y);
                break;
            default:
                break;
        }
    }
    InputManager::update();
}

void Game::update(float dt) {
    // update fps
    Game::fps = 1.0f / dt;

    //Game::scene->update(dt);

    // draw gui
    /*if(!guiStack.empty()){
        GUIHelper::begin();
        for(auto ui: guiStack){
            ui->draw();
        }
        GUIHelper::end();
    }*/
        
}

void Game::render() {
    auto renderer = Game::renderer->getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderClear(renderer);
    //Game::scene->render(Game::renderer);

    //if(!guiStack.empty())
    //    GUIHelper::handleRender(Game::renderer);
    SDL_RenderPresent(renderer);
}

