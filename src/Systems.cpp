#include "Systems.h"
#include "Game.h"
#include "Tile.h"

#include <unordered_set>
#include <functional>

namespace std {
template <>
struct hash<glm::ivec2> {
  size_t operator()(const glm::ivec2& k) const {
    return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
  }
};
} // namespace std

/// returns an empty path on failure
std::vector<glm::ivec2> MoveSystem::findPath(Region& region, glm::ivec2 start, glm::ivec2 end) {
  using P = glm::ivec2;

  std::deque<P> alive;
  std::unordered_set<P> dead;
  alive.push_back(start);

  auto valid = [&region](P p) -> bool {
    auto bounds = p.x >= 0 && p.y >= 0 && p.x < World::world_size && p.y < World::world_size;
    return bounds && TileProperties::of(region[p.x][p.y]).walkable;
  };

  auto seen = [&alive, &dead](P p) -> bool {
    return std::find(alive.begin(), alive.end(), p) != alive.end() || dead.count(p) > 0;
  };

  constexpr auto ws = World::world_size;

  std::vector<std::vector<P>> backtrace(ws, std::vector(ws, P(-1, -1)));

  while (not alive.empty()) {
    auto curr = alive.front();
    alive.pop_front();

    if (curr == end) {
      break;
    }
    
    std::vector<P> neighbors = {curr + P(0, 1), curr + P(0, -1), curr + P(1, 0), curr + P(-1, 0)};

    // get valid neighbors
    for (auto& n : neighbors) {
      if (valid(n) && not seen(n)) {
        backtrace[n.x][n.y] = curr;
        alive.push_back(n);
      }
    }
    dead.insert(curr);
  }

  if (backtrace[end.x][end.y] == P(-1, -1)) {
    return std::vector<P>();
  }

  std::vector<P> trace;
  auto curr = end;
  while (curr != start) {
    trace.push_back(curr);
    if (not valid(curr)) {
      std::puts("error: invalid point on path");
      std::printf("[");
      for (auto i : trace) {
        std::printf(" (%d, %d)", i.x, i.y);
      }
      std::printf(" ]\n");
      exit(0);
    }
    curr = backtrace[curr.x][curr.y];
  }

  std::reverse(trace.begin(), trace.end());

  return trace;
}

void MoveSystem::updateEntity(float dt, ECS::Entity entity) {
  auto& pos = ECS::Manager::getComponent<TransformComponent>(entity).pos;
  MotionComponent& motion = ECS::Manager::getComponent<MotionComponent>(entity);

  if (not motion.hasTarget) return;

  auto& region = motion.world.region();

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

  // like a raytrace for grass
  auto grassCast = [&region](glm::vec2 source, glm::vec2 target) -> bool {
    auto path = target - source;
    auto l = glm::length(path);
    auto dir = glm::normalize(path);
    for (float k = 0; k < l; k += 0.2 /* <- precision */) {
      glm::ivec2 p = Game::mapCoordsToTile(source + dir * k);
      if (not TileProperties::of(region[p.x][p.y]).walkable) {
        return false;
      }
    }
    return true;
  };

  auto seesPoint = [&grassCast, &region, pos](glm::vec2 target) -> bool {
    auto dir = glm::normalize(target - pos);
    auto y = dir.y;
    auto x = dir.x;

    // get the two points on the perpendicular diameter
    auto left = glm::vec2(-y, x) * Unit::unit_size;
    auto right = glm::vec2(y, -x) * Unit::unit_size;

    return grassCast(pos, target) && grassCast(pos + left, target) && grassCast(pos + right, target);
  };

  auto valid = [](glm::ivec2 p) -> bool {
    return 0 <= p.x && 0 <= p.y && p.x < World::world_size && p.y < World::world_size;
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

  // possibly more optimal path finding
  // auto seesTile = [&seesPoint](glm::ivec2 target) -> bool {
  //   glm::vec2 t {target.x + 0.5, target.y + 0.5};
  //   auto dir = glm::normalize(target - pos);
  //   auto y = dir.y;
  //   auto x = dir.x;

  //   // get the two points on the perpendicular diameter
  //   auto left = glm::vec2(-y, x) * Unit::unit_size;
  //   auto right = glm::vec2(y, -x) * Unit::unit_size;

  //   return grassCast(pos + left, target + left) && grassCast(pos + right, target + right);
  // };

  // TODO: investigate only iterating once target location is reached

  // iterate current target until it matches the last one possible to be seen.
  for (auto iter = motion.currentTarget; iter < motion.path.end(); ++iter) {
    glm::ivec2 p = *iter;
    if (seesTile(p)) {
      motion.currentTarget = iter;
    } else {
      break;
    }
  }
  auto target = Game::centerOfTile(*motion.currentTarget);
  
  updatePosition(target);
}