#include "player_system.h"
#include "engine/components/input.h"
#include "engine/components/transform.h"
#include "engine/components/sprite.h"

#include "engine/game.h"

void PlayerSystem::createPlayer(){
    auto ecs = Game::getECS();
    player = ecs->createEntity();
    ecs->addComponent<Transform>(player, Transform(50, 50));
    ecs->addComponent<Sprite>(player, Sprite("assets/test.png"));
}

void PlayerSystem::update(float dt){
    auto ecs = Game::getECS();
    auto input = Game::getInput();
    auto camera = Game::getCamera();
    auto &tf = ecs->query<Transform>(player);
    float v = 200;
    if (input->isKeyHold(Input::Key::A)){
        tf.x -= v * dt;
    }
    if (input->isKeyHold(Input::Key::D)){
        tf.x += v * dt;
    }
    if (input->isKeyHold(Input::Key::W)){
        tf.y -= v * dt;
    }
    if (input->isKeyHold(Input::Key::S)){
        tf.y += v * dt;
    }
    camera->update(tf.x, tf.y);
            
}