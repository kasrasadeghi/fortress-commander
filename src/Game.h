#pragma once

#include <SFML/Graphics.hpp>

enum class Tile { GRASS, WATER };

class World : public sf::Drawable {
  std::vector<std::vector<Tile>> _region; // this should be a square

  void _region_draw(sf::RenderTarget& window) const;

public:
  constexpr static float tile_size = 1.f;

  World(size_t size) : _region(size, std::vector<Tile>(size, Tile::GRASS)) {}
  size_t size() const { return _region.size(); }

  Tile flipCell(sf::Vector2i v) {
    return _region[v.x][v.y] = (_region[v.x][v.y] == Tile::GRASS) ? Tile::WATER : Tile::GRASS;
  }

  void setCell(sf::Vector2i v, Tile t) {
    _region[v.x][v.y] = t;
  }

  virtual void draw(sf::RenderTarget& rw, sf::RenderStates states) const {
    _region_draw(rw);
  };
};

class Game {
  World _world;
  sf::View _view;
  sf::RenderWindow _window;
  sf::Clock _clock;
  Tile _paint = Tile::GRASS;

public:
  constexpr static int view_size = 10;
  static float widthScalingFactor() {
    return 1.f * sf::VideoMode::getFullscreenModes()[0].width /
           sf::VideoMode::getFullscreenModes()[0].height;
  }
  Game();

  sf::Vector2i getMouseTile() {
    sf::Vector2f coords = _window.mapPixelToCoords(sf::Mouse::getPosition(), _view);
    return sf::Vector2i(static_cast<int>(coords.x / World::tile_size), static_cast<int>(coords.y / World::tile_size));
  }

  void loop();
  void handleEvent(const sf::Event& event);

  void handleViewInput(const sf::Time& dt);
};
