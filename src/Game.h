#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>

#include "ECS/Event.h"
#include "ECS/Manager.h"

#include "Components.h"
#include "Systems.h"
#include "World.h"


class Game : public ECS::EventSubscriber<KeyDownEvent>,
                    ECS::EventSubscriber<MouseDownEvent>,
                    ECS::EventSubscriber<MouseMoveEvent> {
  GameState _gameState;
  sf::Font _font;
  World _world;
  sf::View _view;
  sf::RenderWindow _window;
  sf::Clock _clock;
  Tile _paint = Tile::GRASS;

  ECS::Manager _manager;
  ECS::EventManager _eventManager;

  UnitSelectSystem* _unitSelectSystem;
  UnitCommandSystem* _unitCommandSystem;
  MoveSystem* _moveSystem;

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

  void receive(ECS::EventManager* mgr, const KeyDownEvent& e) override;
  void receive(ECS::EventManager* mgr, const MouseDownEvent& e) override;
  void receive(ECS::EventManager* mgr, const MouseMoveEvent& e) override;
  void handleEvent(const sf::Event& event);

  void handleViewInput(const sf::Time& dt);
};
