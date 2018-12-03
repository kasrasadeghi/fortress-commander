#pragma once

#include "Graphics.h"
#include <iostream>
#include <sstream>

#include "ECS/Event.h"
#include "ECS/Manager.h"

#include "Components.h"
#include "Systems.h"
#include "World.h"

class Game : public ECS::EventSubscriber<KeyDownEvent>,
             ECS::EventSubscriber<MouseDownEvent>,
             ECS::EventSubscriber<MouseMoveEvent> {
  RenderWindow _window;
  GameState _gameState;
  World _world;
  Tile _paint = Tile::GRASS;
  bool _debug = false;

  UnitSelectSystem* _unitSelectSystem;
  UnitCommandSystem* _unitCommandSystem;
  UnitCollisionSystem* _unitCollisionSystem;
  MoveSystem* _moveSystem;
  BattleSystem* _battleSystem;

  void _mouseViewMove(float d) {
    auto& _view = _gameState._view;
    constexpr int margin = 20;
    auto pos = _window.mousePos();
    // clang-format off
    if (pos.x < margin) { _view.move(-d, 0); }
    if (pos.y < margin) { _view.move(0, -d); }

    if (pos.x > _window.width() - margin) { _view.move(d, 0); }
    if (pos.y > _window.height() - margin) { _view.move(0, d); }
    // clang-format on
  }

  void _keyboardViewMove(float d) {
    auto& _view = _gameState._view;

    // clang-format off
    if (_window.getKey(GLFW_KEY_W) == GLFW_PRESS) { _view.move(0, -d); }
    if (_window.getKey(GLFW_KEY_S) == GLFW_PRESS) { _view.move(0, d); }
    if (_window.getKey(GLFW_KEY_A) == GLFW_PRESS) { _view.move(-d, 0); }
    if (_window.getKey(GLFW_KEY_D) == GLFW_PRESS) { _view.move(d, 0); }
    // clang-format on
  }

  void _reboundViewToWorld() {
    auto& _view = _gameState._view;

    auto topLeft = _view.center() - _view.radius();
    auto bottomRight = _view.center() + _view.radius();

    const auto viewRadius = world_size * tile_view_size / 2.f;
    const auto worldBorder = world_size * tile_size;
    const auto kw = _window.widthScalingFactor();

    if (topLeft.x < 0) {
      _view.center(viewRadius * kw, _view.center().y);
    }
    if (topLeft.y < 0) {
      _view.center(_view.center().x, viewRadius);
    }
    if (bottomRight.x > worldBorder) {
      _view.center(worldBorder - (viewRadius * kw), _view.center().y);
    }
    if (bottomRight.y > worldBorder) {
      _view.center(_view.center().x, worldBorder - viewRadius);
    }
  }

public:
  static int tile_view_size; // Initialized to 25

  Game();

  glm::vec2 getMouseCoords() {
    return pixelToCoords(_window.mousePos());
  }

  glm::vec2 pixelToCoords(glm::vec2 p) {
    auto& _view = _gameState._view;

    // coords = V' W v
    // W : window -> opengl
    // V : game   -> opengl so V' : opengl -> game
    return _view.inv() * _window.defaultView().proj() * glm::vec4(p.x, p.y, 0, 1);
  }

  static glm::ivec2 mapCoordsToTile(glm::vec2 coords) {
    return glm::ivec2(static_cast<int>(coords.x / tile_size),
                      static_cast<int>(coords.y / tile_size));
  }

  static glm::vec2 centerOfTile(glm::vec2 p) {
    return glm::floor(p) + glm::vec2(0.5 * tile_size, 0.5 * tile_size);
  }

  static glm::vec2 centerOfTile(glm::ivec2 p) {
    return glm::vec2(p.x, p.y) + glm::vec2(0.5 * tile_size, 0.5 * tile_size);
  }

  glm::vec<2, int> getMouseTile() {
    return mapCoordsToTile(getMouseCoords());
  }

  void restart();
  void loop();

  void receive(const KeyDownEvent& e) override;
  void receive(const MouseDownEvent& e) override;
  void receive(const MouseMoveEvent& e) override;

  void keyCallback(int key, int scancode, int action, int mods);
  void mouseCallback(int button, int action, int mods);
  void cursorCallback(double x, double y);

  void handleTick(float dt);

  void incrementZoom();
  void decrementZoom();
};
