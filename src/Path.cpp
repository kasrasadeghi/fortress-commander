#include "Path.h"
#include "GlmHashes.h"
#include "World.h"
#include "Config.h"

#include <unordered_set>
#include <deque>
#include <vector>
#include <queue>

struct FScoreComparator : public std::less<glm::ivec2> {
  std::unordered_map<glm::ivec2, float>& fScore;

  FScoreComparator(std::unordered_map<glm::ivec2, float>& fScore) : fScore(fScore) {}
  
  bool operator()(const glm::ivec2& lhs, const glm::ivec2& rhs) {
    fScore.try_emplace(lhs, std::numeric_limits<float>::infinity()); // default to inf
    fScore.try_emplace(rhs, std::numeric_limits<float>::infinity()); // default to inf
    return fScore.at(lhs) > fScore.at(rhs);
  }
};

Path findPath(Region& region, glm::ivec2 start, glm::ivec2 end) {
  using P = glm::ivec2;

  std::unordered_map<P, P> cameFrom;

  std::unordered_map<P, float> fScore;
  std::unordered_map<P, float> gScore;

  std::unordered_set<P> closed;
  std::unordered_set<P> open;

  FScoreComparator fComparator(fScore); 
  std::priority_queue<P, std::vector<P>, FScoreComparator> openQueue(fComparator);

  auto backtrace = [&cameFrom, &start](P end){
    Path path;
    while (end != start) {
      path.emplace_front(end);
      end = cameFrom[end];
    }
    return path;
  };

  auto heuristic = [&end](const P& p){
    return glm::distance(glm::vec2(p), glm::vec2(end));
  };

  auto valid = [&region](P p) -> bool {
    auto inBounds = p.x >= 0 && p.y >= 0 && p.x < world_size && p.y < world_size;
    return inBounds && TileProperties::of(region[p.x][p.y]).walkable;
  };

  auto direction = [](const P& from, const P& to) {
    P diff = to - from;
    return P{
      diff.x > 0 ? 1 : (diff.x < 0 ? -1 : 0),
      diff.y > 0 ? 1 : (diff.y < 0 ? -1 : 0)
    };
  };

  auto diagonal = [](const P& dir) { return dir.x != 0 && dir.y != 0; };

  auto rotateR = [](const P& p) { return P{-p.y, p.x}; };
  auto rotateL = [](const P& p) { return P{p.y, -p.x}; };

  auto neighborsOf = [](const P& x) {
    return std::vector<P>{
      x + P{1, 0},
      x + P{-1, 0},
      x + P{0, 1},
      x + P{0, -1},
      x + P{1, 1},
      x + P{-1, 1},
      x + P{1, -1},
      x + P{-1, -1}
    };
  };

  // requires that x has a parent (x is not start)
  auto prunedNeighbors = [&](const P& parent, const P& x){
    std::vector<P> neighbors;
    const P dir = direction(parent, x);
    if (diagonal(dir)) {
      const P dx = P{dir.x, 0};
      const P dy = P{0, dir.y};
      if (valid(x + dir)) { neighbors.push_back(x + dir); }
      if (valid(x + dx)) { neighbors.push_back(x + dx); }
      if (valid(x + dy)) { neighbors.push_back(x + dy); }
      if (!valid(x - dx)) { neighbors.push_back(x - dx + dy); }
      if (!valid(x - dy)) { neighbors.push_back(x - dy + dx); }
    } else {
      if (valid(x + dir)) { neighbors.push_back(x + dir); }
      if (!valid(x + rotateL(dir))) { neighbors.push_back(x + rotateL(dir) + dir); }
      if (!valid(x + rotateR(dir))) { neighbors.push_back(x + rotateR(dir) + dir); }
    }
    return neighbors;
  };

  auto jump = [&start, &end](const P& x, const P& dir){
    return x;
  };

  fScore[start] = heuristic(start);
  gScore[start] = 0;

  open.insert(start);
  openQueue.push(start);

  while (not openQueue.empty()) {
    P current = openQueue.top();
    gScore.try_emplace(current, std::numeric_limits<float>::infinity()); // default to inf
    
    if (current == end) {
      return backtrace(current); 
    }

    openQueue.pop();
    open.erase(current);
    closed.insert(current);

    std::vector<P> neighbors;
    if (cameFrom.count(current) > 0) { neighbors = prunedNeighbors(cameFrom[current], current); }
    else {neighbors = neighborsOf(current); }

    for (P neighbor : neighbors) { 
      if (closed.count(neighbor) > 0) {
        continue;
      }
      if (not valid(neighbor)) {
        continue;
      }

      neighbor = jump(neighbor, direction(current, neighbor));

      gScore.try_emplace(neighbor, std::numeric_limits<float>::infinity()); // default to inf

      float tentativeG = gScore.at(current) + 1;
      if (open.count(neighbor) == 0) {
        openQueue.push(neighbor);
        open.insert(neighbor);
      } else if (tentativeG >= gScore.at(neighbor)) {
        continue;
      }

      cameFrom[neighbor] = current;
      gScore[neighbor] = tentativeG;
      fScore[neighbor] = gScore.at(neighbor) + heuristic(neighbor);
    }
  }

  return Path();
}