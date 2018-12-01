#include "GameState.h"
#include "Game.h"

GameState::GameState(RenderWindow& window, std::vector<Unit>& units, std::vector<Enemy>& enemies)
    : _window(window), units(units), enemies(enemies) {
  _view
      // .center(Game::view_size / 2.f * _window.widthScalingFactor(), Game::view_size / 2.f)
      .center(world_size / 2.f, world_size / 2.f)
      .radius(Game::view_size / 2.f * _window.widthScalingFactor(), Game::view_size / 2.f);
}
