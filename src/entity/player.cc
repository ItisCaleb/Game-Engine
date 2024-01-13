#include "player.h"

#include <SDL2/SDL.h>

Player::Player()
    : x(640), y(360), width(50), height(50) {}
Player::~Player() {}
void Player::update(float dt) {
}
void Player::render(SDL_Renderer *renderer) {
    SDL_Rect rect = {.x = this->x - width / 2, .y = this->y - height / 2,
                     .w = this->width, .h = this->height};
    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
}
