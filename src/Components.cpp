#include "Components.h"

void MotionComponent::pathTo(sf::Vector2f pos) {
  // TODO: pathfinding
  target = pos;
  hasTarget = true;
}