#include "game.h"

void Game::init(SDL_Renderer *renderer, SDL_Window *window, int width, int height){
    if (Game::already_init) return;

    Game::already_init = true;
    Game::renderer = renderer;
    Game::window = window;
    Game::width = width;
    Game::height = height;
    Game::camera = Camera(width, height);

}

Player* Game::getPlayer() {
    return currentPlayer;
}

Camera& Game::getCamera() {
    return camera;
}

Scene* Game::getScene() {
    return scene;
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

void Game::update(float dt) {
    Game::scene->update(dt);
}

void Game::render() {
    SDL_SetRenderDrawColor(Game::renderer, 0, 255, 255, 255);
    SDL_RenderClear(Game::renderer);
    Game::scene->render(Game::renderer);
    for (auto s : Game::shapes) {
        s->render(Game::renderer);
    }
    SDL_RenderPresent(Game::renderer);
}

SDL_Window* Game::getWindow() {
    return Game::window;
}
SDL_Renderer* Game::getRenderer() {
    return Game::renderer;
}

int Game::getWidth(){
    return Game::width;
}

int Game::getHeight(){
    return Game::height;
}

 Object* Game::getObjectByShape(CollideShape *shape){
    auto result = Game::shapeToObject.find(shape);
    if (result == Game::shapeToObject.end())
        return nullptr;
 
    return Game::shapeToObject[shape];
 }

std::vector<CollideShape*>* Game::getCollided(CollideShape *shape){
    auto v = new std::vector<CollideShape*>;
    for(auto s: Game::shapes){
        if(shape == s) continue;
        if(shape->isCollide(s)) v->push_back(s);
    }
    return v;
}