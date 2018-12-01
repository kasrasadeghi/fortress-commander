#include "Region.h"

bool Region::structureAt(glm::ivec2 cell) {
  return _structure_pos_set.count(cell) > 0;
}

void Region::addStructure(glm::ivec2 cell) {
  _structure_pos_set.emplace(cell);
}