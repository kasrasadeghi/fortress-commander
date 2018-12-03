#include "Components.h"
#include "Game.h"
#include "Unit.h"
#include "World.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/projection.hpp>

void TransformComponent::translate(glm::vec2 displacement) {
  using V = glm::vec2;
  constexpr float precision = 0.1; // in unit coordinates
  Region& region = world.region();

  V step = glm::normalize(displacement) * precision;
  float steps = glm::length(displacement) / precision;

  auto validPosition = [&](const glm::vec2& pos) {
    if (pos.x < 0 || pos.y < 0 || pos.x >= world_size || pos.y > world_size) {
      return false;
    }
    const glm::ivec2 tilePos = Game::mapCoordsToTile(pos);
    return TileProperties::of(region.at(tilePos)).walkable && not world.structureAt(tilePos).has_value();
  };

  auto intersectsTile = [&](const glm::vec2& pos, const glm::ivec2& tile) {
    const float half_size = tile_size / 2.f;
    auto clamp = [](float f, float lo, float hi) { return std::max(lo, std::min(f, hi)); };
    glm::vec2 tileCenter = Game::centerOfTile(tile);
    glm::vec2 nearest(
      clamp(pos.x, tileCenter.x - half_size, tileCenter.x + half_size),
      clamp(pos.y, tileCenter.y - half_size, tileCenter.y + half_size));
    float dist = glm::distance(pos, nearest);
    return dist < Unit::unit_size;
  };

  auto stepPosition = [&](glm::vec2 step) {
    const glm::vec2 newPos = pos + step;
    const glm::ivec2 tile = Game::mapCoordsToTile(newPos);
    const std::array<glm::ivec2, 8> neighbors{glm::ivec2{tile.x - 1, tile.y}, glm::ivec2{tile.x + 1, tile.y}, 
                                              glm::ivec2{tile.x, tile.y - 1}, glm::ivec2{tile.x, tile.y + 1},
                                              glm::ivec2{tile.x + 1, tile.y + 1}, glm::ivec2{tile.x + 1, tile.y - 1}, 
                                              glm::ivec2{tile.x - 1, tile.y + 1}, glm::ivec2{tile.x - 1, tile.y - 1}};
    for (const glm::ivec2& t : neighbors) {
      if (not validPosition(t) && intersectsTile(newPos, t)) {
        return false;
      }
    } 
    pos = newPos;
    return true;
  };

  // big steps
  while (steps >= 1) {
    stepPosition({step.x, 0});
    stepPosition({0, step.y});
    steps -= 1;
  }

  // final partial step
  step *= steps;
  stepPosition({step.x, 0});
  stepPosition({0, step.y});
}

void MotionComponent::pathTo(glm::vec2 pos) {
  target = Game::centerOfTile(pos);
  path.clear();
  hasTarget = true;
}
