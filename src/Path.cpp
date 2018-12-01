#include "Path.h"
#include "GlmHashes.h"
#include "World.h"

#include <unordered_set>
#include <deque>
#include <vector>


Path findPath(Region& region, glm::ivec2 start, glm::ivec2 end) {
  using P = glm::ivec2;

  using Grid = std::array<std::array<unsigned char, 100>, 100>;

  std::deque<P> alive;
  Grid aliveSet = {0};
  
  Grid dead = {0};
  alive.push_back(start);
  aliveSet[start.x][start.y] = 1;

  auto valid = [&region](P p) -> bool {
    auto bounds = p.x >= 0 && p.y >= 0 && p.x < world_size && p.y < world_size;
    return bounds && TileProperties::of(region[p.x][p.y]).walkable;
  };

  auto seen = [&](P p) -> bool {
    return aliveSet[p.x][p.y] || dead[p.x][p.y];
  };

  constexpr auto ws = world_size;

  std::vector<std::vector<P>> backtrace(ws, std::vector(ws, P(-1, -1)));

  while (not alive.empty()) {
    auto curr = alive.front();
    alive.pop_front();
    aliveSet[curr.x][curr.y] = 0;

    if (curr == end) {
      Path trace;
      auto curr = end;
      while (curr != start) {
        trace.push_front(curr);
        curr = backtrace[curr.x][curr.y];
      }

      return trace;
    }
    dead[curr.x][curr.y] = 1;
    
    const std::array<P, 4> neighbors = {curr + P(0, 1), curr + P(0, -1), curr + P(1, 0), curr + P(-1, 0)};

    // get valid neighbors
    for (auto& n : neighbors) {
      if (valid(n) && not seen(n)) {
        backtrace[n.x][n.y] = curr;
        alive.push_back(n);
        aliveSet[n.x][n.y] = 1;
      }
    }
  }

  return Path();
}