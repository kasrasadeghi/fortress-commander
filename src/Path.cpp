#include "Path.h"
#include "GlmHashes.h"
#include "World.h"

#include <unordered_set>
#include <deque>
#include <vector>


Path findPath(Region& region, glm::ivec2 start, glm::ivec2 end) {
  using P = glm::ivec2;

  std::deque<P> alive;
  std::unordered_set<P> aliveSet;
  
  std::unordered_set<P> dead;
  alive.push_back(start);

  auto valid = [&region](P p) -> bool {
    auto bounds = p.x >= 0 && p.y >= 0 && p.x < World::world_size && p.y < World::world_size;
    return bounds && TileProperties::of(region[p.x][p.y]).walkable;
  };

  auto seen = [&](P p) -> bool {
    return aliveSet.count(p) > 0 || dead.count(p) > 0;
  };

  constexpr auto ws = World::world_size;

  std::vector<std::vector<P>> backtrace(ws, std::vector(ws, P(-1, -1)));

  while (not alive.empty()) {
    auto curr = alive.front();
    alive.pop_front();
    aliveSet.erase(curr);

    if (curr == end) {
      Path trace;
      auto curr = end;
      while (curr != start) {
        trace.push_front(curr);
        curr = backtrace[curr.x][curr.y];
      }

      return trace;
    }
    
    const std::vector<P> neighbors = {curr + P(0, 1), curr + P(0, -1), curr + P(1, 0), curr + P(-1, 0)};

    // get valid neighbors
    for (auto& n : neighbors) {
      if (valid(n) && not seen(n)) {
        backtrace[n.x][n.y] = curr;
        alive.push_back(n);
        aliveSet.insert(n);
      }
    }
    dead.insert(curr);
  }

  return Path();
}