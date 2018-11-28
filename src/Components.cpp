#include "Components.h"
#include "Game.h"

void MotionComponent::pathTo(glm::vec2 pos) {
  // TODO: pathfinding
  target = Game::centerOfTile(pos);
  hasTarget = true;
}