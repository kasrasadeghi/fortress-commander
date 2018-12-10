#include "Path.h"
#include "GlmHashes.h"
#include "Grid.h"
#include "World.h"

#include <deque>
#include <unordered_set>
#include <vector>

Path findPath(Region& region, glm::ivec2 start, glm::ivec2 end) {
  using P = glm::ivec2;

  std::deque<P> alive;
  Grid<> aliveSet;
  for (auto& v : aliveSet) {
    v.fill(0);
  }

  Grid<> dead;
  for (auto& v : dead) {
    v.fill(0);
  }

  alive.push_back(start);
  aliveSet[start.x][start.y] = 1;

  auto valid = [&region](P p) -> bool {
    return region.inBounds(p) && TileProperties::of(region[p.x][p.y]).walkable &&
           (not region.structureAt(p));
  };

  auto seen = [&](P p) -> bool { return aliveSet[p.x][p.y] || dead[p.x][p.y]; };

  Grid<P> backtrace;
  for (auto& v : backtrace) {
    v.fill(P(-1, -1));
  }

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

    const std::array<P, 4> neighbors = {curr + P(0, 1), curr + P(0, -1), curr + P(1, 0),
                                        curr + P(-1, 0)};

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
