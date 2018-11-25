#include "Unit.h"
#include "World.h"

uint Unit::ID_counter = 0;

void Unit::pathTo(glm::vec2 coords) {
  _target = coords;
}

void Unit::update(float dt) {
  if (_target == _pos) return;

  const auto dir = glm::normalize(_target - _pos);

  if (glm::distance(_target, _pos) > dt * unit_speed) {
    _pos += dir * dt * unit_speed;
  } else {
    _pos = _target;
  }
}
