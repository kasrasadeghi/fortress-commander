#include "Unit.h"
#include "World.h"


void World::_drawRegion(sf::RenderTarget& window) const {
  for (int i = 0; i < _region.size(); ++i) {
    for (int j = 0; j < _region[0].size(); ++j) {
      sf::RectangleShape r(sf::Vector2f(tile_size, tile_size));
      r.setPosition(sf::Vector2f(i * tile_size, j * tile_size));

      r.setFillColor(Tile::GRASS == _region[i][j] ? sf::Color(60, 150, 40) : sf::Color::Blue);
      window.draw(r);
    }
  }
}

void World::_drawUnits(sf::RenderTarget& window) const {
  for (const Unit& u : _units) {
    window.draw(u);
  }
}