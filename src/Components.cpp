#include "Components.h"

void MotionComponent::pathTo(glm::vec2 pos) {
  // TODO: pathfinding
  target = pos;
  hasTarget = true;
}