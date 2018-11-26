#include "Unit.h"
#include "World.h"
#include "Game.h"

#include <deque>
#include <vector>
#include "stdio.h"

uint Unit::ID_counter = 0;

void Unit::pathTo(glm::vec2 coords) {
  _target = coords;
}

std::vector<glm::ivec2> findPath(World::Region& region, glm::ivec2 start, glm::ivec2 end) {
  using P = glm::ivec2;

  std::deque<P> alive;
  std::deque<P> dead;
  alive.push_back(start);

  auto valid = [&region](P p) -> bool {
    auto bounds = p.x >= 0 && p.y >= 0 && p.x < World::world_size && p.y < World::world_size;
    return bounds && region[p.x][p.y] == Tile::GRASS;
  };

  auto seen = [&alive, &dead](P p) -> bool {
    return std::find(alive.begin(), alive.end(), p) != alive.end() ||
           std::find(dead.begin(), dead.end(), p) != dead.end();
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
    dead.push_back(curr);
  }

  std::vector<P> trace;
  auto curr = end;
  while (curr != start) {
    trace.push_back(curr);
    if (not valid(curr)) {
      std::puts("");
      break;
    }
    curr = backtrace[curr.x][curr.y];
  }

  std::reverse(trace.begin(), trace.end());

  return trace;
}

void Unit::update(float dt, World& world) {
  if (_target == _pos) return;

  World::Region& region = world.region();

  glm::ivec2 curr_cell = Game::mapCoordsToTile(_pos);
  glm::ivec2 target_cell = Game::mapCoordsToTile(_target);

  auto path = findPath(region, curr_cell, target_cell);

  if (path.size() >= 2) {
    auto next = path[1];
    auto target = glm::vec2(next.x + 0.5f, next.y + 0.5f);
    const auto dir = glm::normalize(target - _pos);

    if (glm::distance(target, _pos) > dt * unit_speed) {
      _pos += dir * dt * unit_speed;
    } else {
      _pos = target;
    }
  } else {
    const auto dir = glm::normalize(_target - _pos);

    if (glm::distance(_target, _pos) > dt * unit_speed) {
      _pos += dir * dt * unit_speed;
    } else {
      _pos = _target;
    }
  }
}