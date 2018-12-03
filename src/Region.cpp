#include "Region.h"

bool Region::structureAt(glm::ivec2 cell) const {
  if (not inBounds({cell.x, cell.y})) {
    return false;
  }

  return _structure_pos_set[cell.x][cell.y];
}

void Region::addStructure(glm::ivec2 cell) {
  if (not inBounds({cell.x, cell.y})) {
    return;
  }

  _structure_pos_set[cell.x][cell.y] = 1;
}

void Region::removeStructure(glm::ivec2 cell) {
  if (not inBounds({cell.x, cell.y})) {
    return;
  }

  _structure_pos_set[cell.x][cell.y] = 0;
}

bool Region::inBounds(glm::vec2 p) const {
  return p.x >= 0 && p.y >= 0 && p.x < world_size && p.y < world_size;
}
