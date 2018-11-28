#include "Components.h"

void MotionComponent::pathTo(glm::vec2 pos) {
  // TODO: pathfinding
  target = glm::floor(pos) + glm::vec2(0.5, 0.5);
  hasTarget = true;
}