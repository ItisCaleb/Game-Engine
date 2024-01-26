#include "game.h"

#include "utils/input_manager.h"
#include "gui/gui_helper.h"

void Game::init(SDL_Renderer *renderer, SDL_Window *window,
     int windowWidth, int windowHeight, int width, int height){
    if (Game::already_init) return;
    SDL_RenderSetLogicalSize(renderer, width, height);
    //SDL_RenderSetScale(renderer, (float)width/windowWidth, (float)height/windowHeight);
    Game::camera = new Camera(width, height);
    Game::already_init = true;
    Game::running = true;
    Game::renderer = renderer;
    Game::window = window;
    Game::windowWidth = windowWidth;
    Game::windowHeight = windowHeight;
    Game::logicWidth = width;
    Game::logicHeight = height;
    
    GUIHelper::init();
}


void Game::setPlayer(Player* player) {
    currentPlayer = player;
}

void Game::setScene(Scene *scene) {
    if(Game::scene){
        delete Game::scene;
    }
    Game::scene = scene;
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

void Game::addCollideShape(CollideShape *shape, Object *object) {
    auto result = Game::shapeToObject.find(shape);
    //non exist
    if (result == Game::shapeToObject.end()){
        Game::shapes.push_back(shape);
        Game::shapeToObject[shape] = object;
    }
    
}


void Game::destroy() {
    SDL_DestroyRenderer(Game::renderer);
    SDL_DestroyWindow(Game::window);
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
    Game::scene->update(dt);

    // draw gui
    if(!guiStack.empty()){
        GUIHelper::begin();
        for(auto ui: guiStack){
            ui->draw();
        }
        GUIHelper::end();
    }
        
}

void Game::render() {
    SDL_SetRenderDrawColor(Game::renderer, 0, 255, 255, 255);
    SDL_RenderClear(Game::renderer);
    Game::scene->render(Game::renderer);
    for (auto s : Game::shapes) {
        s->render(Game::renderer);
    }
    if(!guiStack.empty())
        GUIHelper::handleRender(Game::renderer);
    SDL_RenderPresent(Game::renderer);
}


Object* Game::getObjectByShape(CollideShape *shape){
    auto result = Game::shapeToObject.find(shape);
    if (result == Game::shapeToObject.end())
        return nullptr;
 
    return Game::shapeToObject[shape];
 }

void Game::getCollided(CollideShape *shape, std::vector<CollideShape*> &vec){
    for(auto s: Game::shapes){
        if(shape == s) continue;
        if(shape->isCollide(s)) vec.push_back(s);
    }
}