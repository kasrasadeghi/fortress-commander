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

  auto heuristic = [&end](const P& p){
    return glm::distance(glm::vec2(p), glm::vec2(end));
  };

  auto valid = [&region](P p) -> bool {
    auto inBounds = p.x >= 0 && p.y >= 0 && p.x < world_size && p.y < world_size;
    return inBounds && TileProperties::of(region[p.x][p.y]).walkable;
  };

  std::unordered_map<P, float> fScore;
  std::unordered_map<P, float> gScore;
  fScore[start] = heuristic(start);
  gScore[start] = 0;

  FScoreComparator fComparator(fScore); 

  std::unordered_set<P> closed;
  std::unordered_set<P> open;
  open.insert(start);

  std::priority_queue<P, std::vector<P>, FScoreComparator> openQueue(fComparator);
  openQueue.push(start);

  std::unordered_map<P, P> cameFrom;

  auto backtrace = [&cameFrom, &start](P end){
    Path path;
    while (end != start) {
      path.emplace_front(end);
      end = cameFrom[end];
    }
    return path;
  };

  while (not openQueue.empty()) {
    P current = openQueue.top();
    gScore.try_emplace(current, std::numeric_limits<float>::infinity()); // default to inf
    
    if (current == end) {
      return backtrace(current); 
    }

    openQueue.pop();
    open.erase(current);
    closed.insert(current);

    const std::vector<P> neighbors = {current + P(0, 1), current + P(0, -1), current + P(1, 0), current + P(-1, 0)};
    for (P neighbor : neighbors) { 
      if (closed.count(neighbor) > 0) {
        continue;
      }
      if (not valid(neighbor)) {
        continue;
      }

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