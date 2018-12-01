#include "Components.h"
#include "Game.h"
#include "Unit.h"
#include "World.h"

void TransformComponent::translate(glm::vec2 displacement) {
  constexpr float border = tile_size * Unit::unit_size;
  pos += displacement;
  pos.x = std::min(world_size * tile_size - border, std::max(border, pos.x));
  pos.y = std::min(world_size * tile_size - border, std::max(border, pos.y));
}

void MotionComponent::pathTo(glm::vec2 pos) {
  target = Game::centerOfTile(pos);
  path.clear();
  hasTarget = true;
}
