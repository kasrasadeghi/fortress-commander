#pragma once

#include "Graphics.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

// #include "World.h"

enum class ControlMode { NONE, BUILD, UNIT, TERRAIN };

class Game {
  // sf::Font _font;
  // World _world;
  // sf::View _view;
  // sf::Clock _clock;
  RenderWindow _window;
  // Tile _paint = Tile::GRASS;

  ControlMode _mode = ControlMode::NONE;

  //   template <typename T>
  //   sf::Text makeText(T obj) {
  //     std::stringstream ss;
  //     ss << obj;
  //     sf::Text t;
  //     t.setFont(_font);
  //     t.setString(ss.str());
  //     t.setCharacterSize(24);
  //     t.setFillColor(sf::Color::Black);

  //     return t;
  //   }

  //   void _mouseViewMove(float d) {
  //     constexpr int margin = 20;
  //     auto pos = sf::Mouse::getPosition();
  //     if (pos.x < margin) { _view.move(-d, 0); }
  //     if (pos.y < margin) { _view.move(0, -d); }

  //     if (pos.x > _window.getSize().x - margin) { _view.move(d, 0); }
  //     if (pos.y > _window.getSize().y - margin) { _view.move(0, d); }
  //   }

  //   void _keyboardViewMove(float d) {
  //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { _view.move(0, -d); }
  //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { _view.move(0, d); }
  //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { _view.move(-d, 0); }
  //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { _view.move(d, 0); }
  //     if (_mode == ControlMode::TERRAIN) {
  //       if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
  //         _world.setCell(getMouseTile(), _paint);
  //       }
  //     }
  //   }

  //   void _reboundViewToWorld() {
  //     auto topLeft = _view.getCenter() - (_view.getSize() / 2.f);
  //     auto bottomRight = _view.getCenter() + (_view.getSize() / 2.f);

  //     const auto viewRadius = (view_size * tile_size) / 2.f;
  //     const auto worldBorder = World::world_size * tile_size;

  //     if (topLeft.x < 0) {
  //       _view.setCenter(viewRadius * widthScalingFactor(),
  //       _view.getCenter().y);
  //     }
  //     if (topLeft.y < 0) { _view.setCenter(_view.getCenter().x, viewRadius);
  //     } if (bottomRight.x > worldBorder) {
  //       _view.setCenter(worldBorder - (viewRadius * widthScalingFactor()),
  //                       _view.getCenter().y);
  //     }
  //     if (bottomRight.y > worldBorder) {
  //       _view.setCenter(_view.getCenter().x, worldBorder - viewRadius);
  //     }
  //   }

public:
  //   constexpr static float view_size = 20 * tile_size;
  //   static float widthScalingFactor() {
  //     return 1.f * sf::VideoMode::getFullscreenModes()[0].width /
  //            sf::VideoMode::getFullscreenModes()[0].height;
  //   }
  Game();

  //   sf::Vector2f getMouseCoords() {
  //     return _window.mapPixelToCoords(sf::Mouse::getPosition(), _view);
  //   }

  //   static sf::Vector2i mapCoordsToTile(sf::Vector2f coords) {
  //     return sf::Vector2i(static_cast<int>(coords.x / tile_size),
  //                         static_cast<int>(coords.y / tile_size));
  //   }

  //   sf::Vector2i getMouseTile() {
  //     sf::Vector2f coords = getMouseCoords();
  //     return mapCoordsToTile(coords);
  //   }

  void loop();

  void keyCallback(int key, int scancode, int action, int mods);
  // void handleEvent(const sf::Event& event);

  // void handleViewInput(const sf::Time& dt);
};
