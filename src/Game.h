#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "World.h"

#include "ECS/Manager.h"

enum class ControlMode { NONE, BUILD, UNIT, TERRAIN };

class Game {
  sf::Font _font;
  World _world;
  sf::View _view;
  sf::RenderWindow _window;
  sf::Clock _clock;
  Tile _paint = Tile::GRASS;

  ECS::Manager _manager;

  ControlMode _mode = ControlMode::NONE;

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

  static sf::Vector2i mapCoordsToTile(sf::Vector2f coords) {
    return sf::Vector2i(static_cast<int>(coords.x / World::tile_size),
                        static_cast<int>(coords.y / World::tile_size));
  }

  sf::Vector2i getMouseTile() {
    sf::Vector2f coords = getMouseCoords();
    return mapCoordsToTile(coords);
  }

  void loop();
  void handleEvent(const sf::Event& event);

  void handleViewInput(const sf::Time& dt);
};
