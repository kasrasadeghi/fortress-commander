#include "Game.h"
#include "Config.h"
#include "Graphics.h"

#include "ECS/Manager.h"
#include "Events.h"
#include "Systems.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <vector>

Game::Game() : _window("Fortress Commander"), _gameState(_window), _world(World::world_size) {
  _window.setKeyCallback([this](auto&&... args) { keyCallback(args...); });
  _window.setMouseCallback([this](auto&&... args) { mouseCallback(args...); });
  _window.setCursorCallback([this](auto&&... args) { cursorCallback(args...); });

  // glfwSwapInterval(1);

  ECS::Manager::getInstance().createComponentStore<TransformComponent>(); 
  ECS::Manager::getInstance().createComponentStore<MotionComponent>();
  ECS::Manager::getInstance().createComponentStore<SelectableComponent>();
  ECS::Manager::getInstance().createComponentStore<CommandableComponent>();

  _moveSystem = new MoveSystem(_eventManager, _gameState);
  ECS::Manager::getInstance().addSystem(ECS::System::Ptr(_moveSystem));

  _unitSelectSystem = new UnitSelectSystem(_eventManager, _gameState);
  ECS::Manager::getInstance().addSystem(ECS::System::Ptr(_unitSelectSystem));

  _unitCommandSystem = new UnitCommandSystem(_eventManager, _gameState);
  ECS::Manager::getInstance().addSystem(ECS::System::Ptr(_unitCommandSystem));

  _eventManager.connect<KeyDownEvent>(this);
  _eventManager.connect<MouseDownEvent>(this);
  _eventManager.connect<MouseMoveEvent>(this);
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

  while (_window.isOpen()) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float dt = glfwGetTime() - last_time;
    last_time = glfwGetTime();

    handleTick(dt);
    _world.draw(_gameState._view);

    auto& _mode = _gameState._mode;
    if (_mode == ControlMode::BUILD || _mode == ControlMode::TERRAIN) {
      World::tileHolo(_gameState._view, getMouseTile());
    }
    if (_mode == ControlMode::UNIT) {
      Unit::holo(_gameState._view, getMouseCoords());
    }

    t.renderText(str(1.f / dt), 100, 50, 1, glm::vec4(0, 0, 0, 1));

    _eventManager.update();
    ECS::Manager::getInstance().update(dt);

    _window.swapBuffers();
    glfwPollEvents();
  }
}

void Game::receive(ECS::EventManager* mgr, const KeyDownEvent& e) {
  auto key = e.key;
  auto action = e.action;

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    _window.close();
  }

  auto& _mode = _gameState._mode;

  // TODO: temporary. normally left clicking on empty ground gets you out of a mode
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    _mode = ControlMode::NONE;
  }

  if (key == GLFW_KEY_B && action == GLFW_PRESS) {
    _mode = ControlMode::BUILD;
  }
  if (key == GLFW_KEY_U && action == GLFW_PRESS) {
    _mode = ControlMode::UNIT;
  }
  if (key == GLFW_KEY_T && action == GLFW_PRESS) {
    _mode = ControlMode::TERRAIN;
  }
}

void Game::receive(ECS::EventManager* mgr, const MouseDownEvent& e) {
  if (_gameState._mode == ControlMode::BUILD) {
    //_world.addStructure(getMouseTile());
  }
  if (_gameState._mode == ControlMode::TERRAIN) {
    _paint = _world.flipCell(getMouseTile());
  }
  if (_gameState._mode == ControlMode::UNIT) {
    // TODO: check if the add unit is in bounds
    _world._units.push_back(Unit(getMouseCoords()));
  }
}

void Game::receive(ECS::EventManager* mgr, const MouseMoveEvent& e) {
  if (_gameState._mode == ControlMode::TERRAIN &&
      glfwGetMouseButton(_window.window(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
    _world.setCell(getMouseTile(), _paint);
  }
}

void Game::keyCallback(int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    _eventManager.event(new KeyDownEvent(key));
  }
  if (action == GLFW_RELEASE) {
    _eventManager.event(new KeyUpEvent(key));
  }
}

void Game::mouseCallback(int button, int action, int mods) {
  if (action == GLFW_PRESS) {
    _eventManager.event(new MouseDownEvent(button, getMouseCoords().x, getMouseCoords().y));
  }
  if (action == GLFW_RELEASE) {
    _eventManager.event(new MouseUpEvent(button, getMouseCoords().x, getMouseCoords().y));
  }
}

void Game::cursorCallback(double x, double y) {
  // TODO: move to receive
  if (_gameState._mode == ControlMode::TERRAIN &&
      glfwGetMouseButton(_window.window(), GLFW_MOUSE_BUTTON_1)) {
    _world.setCell(getMouseTile(), _paint);
  }
  _eventManager.event(new MouseMoveEvent(getMouseCoords().x, getMouseCoords().y));
}

void Game::handleTick(float dt) {
  constexpr float speed = 20 * tile_size;
  float d = speed * dt;

  _mouseViewMove(d);

  _keyboardViewMove(d);

  // lock view to world by rebounding
  _reboundViewToWorld();
}
