#include "GameState.h"
#include "Game.h"

GameState::GameState(RenderWindow& window) : _window(window) {
  _view
    // .center(Game::view_size / 2.f * _window.widthScalingFactor(), Game::view_size / 2.f)
    .center(World::world_size / 2.f, World::world_size / 2.f)
    .radius(Game::view_size / 2.f * _window.widthScalingFactor(), Game::view_size / 2.f);
}