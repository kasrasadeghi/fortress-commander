#include "GameState.h"
#include "Game.h"

#include "ParticleSystem.h"

GameState::GameState(RenderWindow& window, std::vector<Unit>& units, std::vector<Enemy>& enemies)
    : _window(window), units(units), enemies(enemies), 
      _bulletParticles(_view, BulletParticle::beforeUpdate, BulletParticle::afterUpdate) {
  _view
      .center(world_size / 2.f, world_size / 2.f)
      .radius(Game::tile_view_size * tile_size / 2.f * _window.widthScalingFactor(), Game::tile_view_size * tile_size / 2.f);
}
