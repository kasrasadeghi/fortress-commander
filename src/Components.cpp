#include "Components.h"
#include "Game.h"

void MotionComponent::pathTo(glm::vec2 pos) {
  target = Game::centerOfTile(pos);
  path.clear();
  hasTarget = true;
}
