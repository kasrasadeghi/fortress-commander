#include "World.h"
#include "Graphics.h"
#include "Unit.h"

void World::_drawRegion(View view) const {
  for (int i = 0; i < world_size; ++i) {
    for (int j = 0; j < world_size; ++j) {
      RectangleShape r;
      r.size(tile_size, tile_size).position(i * tile_size, j * tile_size);

      // r.color(Tile::GRASS == _region[i][j] ? glm::vec3(60.f / 255.f, 150.f / 255.f, 40.f / 255.f)
      // : glm::vec3(0, 0, 1)); window.draw(r);
      r.draw(view);
    }
  }
}

// void World::_drawUnits(sf::RenderTarget& window) const {
//   for (const Unit& u : _units) { window.draw(u); }
// }