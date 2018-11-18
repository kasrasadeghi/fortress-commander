#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "World.h"

class BuildManager : public sf::Drawable {
  /// layouts assumed to be rectangular, at least containing zeros
  /// center of mass assumed to be similar to layout if all values were 1
  sf::Vector2i _currTile;

public:
  void setMouseTile(sf::Vector2i coords) { _currTile = coords; }

  virtual void draw(sf::RenderTarget& t, sf::RenderStates states) const {
    sf::RectangleShape r(sf::Vector2f(World::tile_size, World::tile_size));
    r.setPosition(_currTile.x * World::tile_size,
                  _currTile.y * World::tile_size);
    r.setFillColor(sf::Color(255, 200, 200, 200));
    t.draw(r);
  }
};

enum class ControlMode { NONE, BUILD, UNIT };

class Game {
  sf::Font _font;
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
    return sf::Vector2i(static_cast<int>(coords.x / World::tile_size),
                        static_cast<int>(coords.y / World::tile_size));
  }

  void loop();
  void handleEvent(const sf::Event& event);

  void handleViewInput(const sf::Time& dt);
};
