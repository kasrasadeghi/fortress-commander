#include "Systems.h"
#include "Game.h"
#include "Tile.h"

#include "GlmHashes.h"
#include "Path.h"

#include <unordered_set>

void MoveSystem::updateEntity(float dt, ECS::Entity entity) {
  auto& pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
  MotionComponent& motion = ECS::Manager::getComponent<MotionComponent>(entity);
  auto& world = ECS::Manager::getComponent<TransformComponent>(entity).world;

  if (not motion.hasTarget) return;

  auto& region = world.region();

  if (motion.path.empty()) {
    glm::ivec2 curr_cell = Game::mapCoordsToTile(pos);
    glm::ivec2 target_cell = Game::mapCoordsToTile(motion.target);

    auto path = findPath(region, curr_cell, target_cell);
    if (path.empty()) { // pathfinding failed
      motion.hasTarget = false;
      return;
    }
    motion.path = std::move(path);

    motion.currentTarget = motion.path.begin();
  }

  auto raycast = [&region](glm::vec2 source, glm::vec2 target) -> bool {
    auto path = target - source;
    auto l = glm::length(path);
    auto dir = glm::normalize(path);
    for (float k = 0; k < l; k += 0.2 /* <- precision */) {
      glm::ivec2 p = Game::mapCoordsToTile(source + dir * k);
      if (not TileProperties::of(region[p.x][p.y]).walkable || region.structureAt(p)) {
        return false;
      }
    }
    return true;
  };

  auto seesPoint = [&raycast, &region, pos](glm::vec2 target) -> bool {
    auto dir = glm::normalize(target - pos);
    auto y = dir.y;
    auto x = dir.x;

    // get the two points on the perpendicular diameter
    auto left = glm::vec2(-y, x) * Unit::unit_size;
    auto right = glm::vec2(y, -x) * Unit::unit_size;

    return raycast(pos, target) && raycast(pos + left, target) && raycast(pos + right, target);
  };

  auto valid = [](glm::ivec2 p) -> bool {
    return 0 <= p.x && 0 <= p.y && p.x < world_size && p.y < world_size;
  };

  auto seesTile = [&seesPoint, &valid](glm::ivec2 target) -> bool {
    glm::vec2 t;
    glm::ivec2 curr;

    curr = target;
    t = {curr.x, curr.y};
    if (valid(curr) && not seesPoint(t)) {
      return false;
    }

    curr = target + glm::ivec2(0, 1);
    t = {curr.x, curr.y};
    if (valid(curr) && not seesPoint(t)) {
      return false;
    }

    curr = target + glm::ivec2(1, 0);
    t = {curr.x, curr.y};
    if (valid(curr) && not seesPoint(t)) {
      return false;
    }

    curr = target + glm::ivec2(0, 1);
    t = {curr.x, curr.y};
    if (valid(curr) && not seesPoint(t)) {
      return false;
    }

    return true;
  };
  // TODO: investigate only iterating once target location is reached

  // iterate from current target until the last one possible to be seen
  // set the current target to that last one
  for (auto iter = motion.currentTarget; iter < motion.path.end(); ++iter) {
    glm::ivec2 p = *iter;
    if (seesTile(p)) {
      motion.currentTarget = iter;
    }
  }
  auto target = Game::centerOfTile(*motion.currentTarget);

  auto updatePosition = [dt, &pos, &motion](glm::vec2 target) {
    if (glm::distance(target, pos) > dt * motion.movementSpeed) {
      auto dir = glm::normalize(target - pos);
      pos += dir * dt * motion.movementSpeed;
    } else {
      pos = target;
      if (target == motion.target) {
        motion.hasTarget = false;
        motion.path.clear();
      }
    }
  };

  updatePosition(target);
}
