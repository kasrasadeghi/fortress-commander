#include "Systems.h"
#include "Game.h"
#include "Tile.h"

#include "GlmHashes.h"
#include "Path.h"

#include <unordered_set>

bool MoveSystem::tileVisible(Region& region, glm::ivec2 tileCoord, glm::vec2 from) {
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

  auto seesPoint = [&raycast, &region, &from](glm::vec2 target) -> bool {
    return raycast(from, target);
  };

  auto valid = [](glm::ivec2 p) -> bool {
    return 0 <= p.x && 0 <= p.y && p.x < world_size && p.y < world_size;
  };

  glm::vec2 t;
  glm::ivec2 curr;

  curr = tileCoord;
  t = {curr.x, curr.y};
  if (valid(curr) && not seesPoint(t)) {
    return false;
  }

  curr = tileCoord + glm::ivec2(0, 1);
  t = {curr.x, curr.y};
  if (valid(curr) && not seesPoint(t)) {
    return false;
  }

  curr = tileCoord + glm::ivec2(1, 0);
  t = {curr.x, curr.y};
  if (valid(curr) && not seesPoint(t)) {
    return false;
  }

  curr = tileCoord + glm::ivec2(0, 1);
  t = {curr.x, curr.y};
  if (valid(curr) && not seesPoint(t)) {
    return false;
  }

  return true;
}

void MoveSystem::recomputePath(ECS::Entity entity) {
  auto& pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
  auto& region = ECS::Manager::getComponent<TransformComponent>(entity).world.region();
  auto& motion = ECS::Manager::getComponent<MotionComponent>(entity);

  glm::ivec2 curr_cell = Game::mapCoordsToTile(pos);
  glm::ivec2 target_cell = Game::mapCoordsToTile(motion.target);

  auto path = findPath(region, curr_cell, target_cell);
  if (path.empty()) { // pathfinding failed
    motion.hasTarget = false;
    return;
  }

  motion.path = std::move(path);

  auto simplifyPath = [&](Path& path) {
    if (path.size() == 0)
      return Path();

    Path simplifiedPath;
    // simplifiedPath.push_back(path[0]);

    uint last = 0;
    uint index = 1;
    while (index < path.size()) {
      if (tileVisible(region, path[index], path[last])) {
        ++index;
      } else {
        last = index - 1; // update last visible point
        simplifiedPath.push_back(path[last]); // emit the point
        // ++index; // TODO: remove; ensures algorithm progression but produces extra steps
                 // might need to adjust tileVisible to avoid infinite loop here.
      }
    }
    simplifiedPath.push_back(path.back());
    return simplifiedPath;
  }; 
  motion.path = simplifyPath(motion.path);
  motion.oldPosition = pos;
}

void MoveSystem::updateEntity(float dt, ECS::Entity entity) {
  auto& pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
  auto& rot = ECS::Manager::getComponent<TransformComponent>(entity).rot;
  auto& motion = ECS::Manager::getComponent<MotionComponent>(entity);

  if (not motion.hasTarget) {
    return;
  }

  if (motion.path.empty()) {
    recomputePath(entity);
  }

  glm::vec2 targetPos = Game::centerOfTile(motion.path.front());
  float dist = glm::distance(pos, targetPos);

  glm::vec2 pathDir = targetPos - motion.oldPosition;
  float dot = glm::dot(pathDir, targetPos - pos);
  if (dist < Unit::unit_size || dot < 0) {
    // advance to next waypoint
    motion.oldPosition = pos;
    motion.path.pop_front();
    if (motion.path.empty()) {
      motion.hasTarget = false;
    }
  } else {
    // TODO: raycast to next waypoint to ensure that path is still valid, else path to it and prepend to existing path
    // move toward current waypoint
    auto dir = glm::normalize(targetPos - pos);
    rot = -glm::atan(dir.y, dir.x) - glm::half_pi<float>();
    ECS::Manager::getComponent<TransformComponent>(entity).translate(dir * motion.movementSpeed * dt);
  }
}
