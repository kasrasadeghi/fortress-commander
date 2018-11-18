#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Unit.h"

enum class Tile { NONE, GRASS, WATER };

class World : public sf::Drawable {
  std::vector<std::vector<Tile>> _region; // this should be a square
  sf::Vector2f _unit_coords = sf::Vector2f(10.f, 10.f);

  std::vector<Unit> _units;
  //std::vector<Structure> _structures;

  void _drawRegion(sf::RenderTarget& window) const;
  void _drawUnits(sf::RenderTarget& window) const;

  /// stuff to make out of bounds clicks snap back to bounds
  bool _snapToRegion(sf::Vector2i& v) {
    bool result = true;
    if (0 > v.x) {
      v.x = 0;
      result = false;
    } 
    if (v.x >= _region.size()) {
      v.x = _region.size() - 1;
      result = false;
    } 
    if (0 > v.y) {
      v.y = 0;
      result = false;
    }
    if (v.y >= _region.size()) {
      v.y = _region.size() - 1;
      result = false;
    }
    return result;
  }

public:
  constexpr static float tile_size = 1.f;
  constexpr static int world_size = 50;

  World(size_t size) : _region(size, std::vector<Tile>(size, Tile::GRASS)) {}
  size_t size() const { return _region.size(); }

  void addUnit(Unit u) {
    _units.push_back(u);
  }

  Tile flipCell(sf::Vector2i v) {
    _snapToRegion(v);
    return _region[v.x][v.y] = (_region[v.x][v.y] == Tile::GRASS) ? Tile::WATER : Tile::GRASS;
  }

  void setCell(sf::Vector2i v, Tile t) {
    _snapToRegion(v);
    _region[v.x][v.y] = t;
  }

  virtual void draw(sf::RenderTarget& rw, sf::RenderStates states) const {
    _drawRegion(rw);
    _drawUnits(rw);
  }
};

class BuildManager : public sf::Drawable {
  /// layouts assumed to be rectangular, at least containing zeros
  /// layouts center of mass assumed to be similar to layout if all values were 1
  sf::Vector2i _currTile;
public:
  void setMouseTile(sf::Vector2i coords) {
    _currTile = coords;
  }

  virtual void draw(sf::RenderTarget& t, sf::RenderStates states) const {
    sf::RectangleShape r(sf::Vector2f(World::tile_size, World::tile_size));
    r.setPosition(_currTile.x * World::tile_size, _currTile.y * World::tile_size);
    r.setFillColor(sf::Color(255, 200, 200, 200));
    t.draw(r);
  }
};

enum class ControlMode {
  NONE, BUILD, UNIT
};

class Game {
  World _world;
  sf::View _view;
  sf::RenderWindow _window;
  sf::Clock _clock;
  Tile _paint = Tile::GRASS;

  ControlMode _mode = ControlMode::NONE;
  BuildManager _buildManager;

public:
  constexpr static int view_size = 20; // how many tiles the view should see
  static float widthScalingFactor() {
    return 1.f * sf::VideoMode::getFullscreenModes()[0].width /
           sf::VideoMode::getFullscreenModes()[0].height;
  }
  Game();

  sf::Vector2f getMouseCoords() {
    return _window.mapPixelToCoords(sf::Mouse::getPosition(), _view);
  }

  sf::Vector2i getMouseTile() {
    sf::Vector2f coords = getMouseCoords();
    return sf::Vector2i(static_cast<int>(coords.x / World::tile_size), static_cast<int>(coords.y / World::tile_size));
  }

  void loop();
  void handleEvent(const sf::Event& event);

  void handleViewInput(const sf::Time& dt);
};
