#include "Game.h"
#include "Config.h"
#include "Graphics.h"

#include "EnemySpawner.h"

#include "ECS/Manager.h"
#include "Events.h"
#include "Systems.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <random>
#include <sstream>
#include <stdio.h>
#include <vector>
 
int Game::tile_view_size = 25;

Game::Game()
    : _window("Fortress Commander"), _gameState(_window, _world._units, _world._enemies),
      _world(world_size) {
  _window.setKeyCallback([this](auto&&... args) { keyCallback(args...); });
  _window.setMouseCallback([this](auto&&... args) { mouseCallback(args...); });
  _window.setCursorCallback([this](auto&&... args) { cursorCallback(args...); });

  glfwSwapInterval(0); // oh, it's on by default

  ECS::Manager::createComponentStore<TransformComponent>();
  ECS::Manager::createComponentStore<MotionComponent>();
  ECS::Manager::createComponentStore<SelectableComponent>();
  ECS::Manager::createComponentStore<CommandableComponent>();
  ECS::Manager::createComponentStore<HealthComponent>();
  ECS::Manager::createComponentStore<AttackComponent>();

  _moveSystem = new MoveSystem(_gameState);
  ECS::Manager::addSystem(ECS::System::Ptr(_moveSystem));

  _unitSelectSystem = new UnitSelectSystem(_gameState);
  ECS::Manager::addSystem(ECS::System::Ptr(_unitSelectSystem));

  _unitCommandSystem = new UnitCommandSystem(_gameState);
  ECS::Manager::addSystem(ECS::System::Ptr(_unitCommandSystem));

  _unitCollisionSystem = new UnitCollisionSystem(_gameState);
  ECS::Manager::addSystem(ECS::System::Ptr(_unitCollisionSystem));

  _battleSystem = new BattleSystem(_gameState);
  ECS::Manager::addSystem(ECS::System::Ptr(_battleSystem));

  ECS::EventManager::connect<KeyDownEvent>(this);
  ECS::EventManager::connect<MouseDownEvent>(this);
  ECS::EventManager::connect<MouseMoveEvent>(this);
}

template <typename T>
std::string str(T obj) {
  std::stringstream ss;
  ss << obj;
  return ss.str();
}

void Game::loop() {
  float last_time = glfwGetTime();
  TextRenderer t(_window.defaultView());
  EnemySpawner spawner(_world);

  while (_window.isOpen()) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float dt = glfwGetTime() - last_time;
    last_time = glfwGetTime();

    handleTick(dt);
    _world.draw(_gameState._view, _debug);

    glm::vec4 modeColor(.9, .9, .1, 1);
    auto& _mode = _gameState._mode;
    if (_mode == ControlMode::BUILD) {
      t.renderText("BUILD", _window.width() - 200, 50, 1, modeColor);
      _world.structHolo(_gameState._view, getMouseTile());
    }
    if (_mode == ControlMode::SELL) {
      t.renderText("SELL", _window.width() - 200, 50, 1, modeColor);
      _world.structHolo(_gameState._view, getMouseTile());
    }
    if (_mode == ControlMode::TERRAIN) {
      t.renderText("TERRAIN", _window.width() - 300, 50, 1, modeColor);
      World::tileHolo(_gameState._view, getMouseTile());
    }
    if (_mode == ControlMode::UNIT) {
      t.renderText("UNIT", _window.width() - 200, 50, 1, modeColor);
      Unit::holo(_gameState._view, getMouseCoords());
    }

    t.renderText(str(1.f / dt), 100, 50, 1, glm::vec4(0, 0, 0, 1));

    ECS::EventManager::update();
    ECS::Manager::update(dt);
    spawner.update(dt);

    _window.swapBuffers();
    glfwPollEvents();
  }
}

void Game::receive(const KeyDownEvent& e) {
  auto key = e.key;

  if (key == GLFW_KEY_ESCAPE) {
    _window.close();
  }

  auto& _mode = _gameState._mode;

  // TODO: temporary. normally left clicking on empty ground gets you out of a mode
  if (key == GLFW_KEY_Q) {
    _mode = ControlMode::NONE;
  }

  if (key == GLFW_KEY_B) {
    _mode = ControlMode::BUILD;
  }
  if (key == GLFW_KEY_X) {
    _mode = ControlMode::SELL;
  }
  if (key == GLFW_KEY_U || key == GLFW_KEY_E) {
    _mode = ControlMode::UNIT;
  }
  if (key == GLFW_KEY_T) {
    _mode = ControlMode::TERRAIN;
  }

  if (key == GLFW_KEY_SEMICOLON) {
    _debug = not _debug;
  }
}

void Game::receive(const MouseDownEvent& e) {
  if (_gameState._mode == ControlMode::BUILD) {
    _world.addStructure(getMouseTile());
  }
  if (_gameState._mode == ControlMode::SELL) {
    _world.sellStructure(getMouseTile());
  }
  if (_gameState._mode == ControlMode::TERRAIN) {
    _paint = _world.flipCell(getMouseTile());
  }
  if (_gameState._mode == ControlMode::UNIT) {
    _world.addUnit(getMouseCoords());
  }
}

void Game::receive(const MouseMoveEvent& e) {
  if (_gameState._mode == ControlMode::TERRAIN &&
      glfwGetMouseButton(_window.window(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
    _world.setCell(getMouseTile(), _paint);
  }
}

void Game::keyCallback(int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    ECS::EventManager::event(new KeyDownEvent(key));
  }
  if (action == GLFW_RELEASE) {
    ECS::EventManager::event(new KeyUpEvent(key));
  }
}

void Game::mouseCallback(int button, int action, int mods) {
  if (action == GLFW_PRESS) {
    ECS::EventManager::event(new MouseDownEvent(button, getMouseCoords().x, getMouseCoords().y));
  }
  if (action == GLFW_RELEASE) {
    ECS::EventManager::event(new MouseUpEvent(button, getMouseCoords().x, getMouseCoords().y));
  }
}

void Game::cursorCallback(double x, double y) {
  ECS::EventManager::event(new MouseMoveEvent(getMouseCoords().x, getMouseCoords().y));
}

void Game::handleTick(float dt) {
  constexpr float speed = 20 * tile_size;
  float d = speed * dt;

  _mouseViewMove(d);

  _keyboardViewMove(d);

  // lock view to world by rebounding
  _reboundViewToWorld();
}
