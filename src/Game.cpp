#include "Game.h"
#include "Config.h"
#include "Graphics.h"

#include "ECS/Manager.h"
#include "Events.h"

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
    : _resources(init_resource_bal), _window("Fortress Commander"), _gameState(_window, _world._units, _world._enemies, _resources),
      _world(world_size, _resources), _spawner(_world) {
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
  ECS::Manager::createComponentStore<ResourceComponent>();

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

  _resourceSystem = new ResourceSystem(_gameState, _world._resources);
  ECS::Manager::addSystem(ECS::System::Ptr(_resourceSystem));

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

void Game::restart() {
  ECS::Manager::clear();

  _resources = init_resource_bal;

  _spawner.reset();

  _world = World(world_size, _resources);
}

void Game::loop() {
  float last_time = glfwGetTime();
  TextRenderer t(_window.defaultView());

  TextureBatch batch(ResourceManager::texture());
  batch.view(_gameState._view);

  RectangleBatch ui;

  while (_window.isOpen()) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float dt = glfwGetTime() - last_time;
    last_time = glfwGetTime();

    handleTick(dt);
    _world.draw(batch, _gameState._view, _debug);
    
    ui.add()
      .position({_window.width() - 150, 0})
      .size({300, 150})
      .color({.3, .3, .3, 1});

    ui.add()
      .position({_window.width() - 300, _window.height() - 30})
      .size({600, 60})
      .color({.3, .3, .3, 1});
    ui.draw(_window._default);

    glm::vec4 modeColor(0.76, 0.27, 0.19, 1);
    auto& _mode = _gameState._mode;
    if (_mode == ControlMode::BUILD) {
      t.renderText("BUILD", _window.width() - 250, 50, 1, modeColor);
      _world.structHolo(_gameState._view, getMouseTile());
    }
    if (_mode == ControlMode::SELL) {
      t.renderText("SELL", _window.width() - 250, 50, 1, modeColor);
      _world.structHolo(_gameState._view, getMouseTile());
    }
    if (_mode == ControlMode::TERRAIN) {
      t.renderText("TERRAIN", _window.width() - 250, 50, 1, modeColor);
      World::tileHolo(_gameState._view, getMouseTile());
    }
    if (_mode == ControlMode::UNIT) {
      t.renderText("UNIT", _window.width() - 250, 50, 1, modeColor);
      Unit::holo(_gameState._view, getMouseCoords());
    }

    if (_debug) {
      t.renderText(str(1.f / dt), 100, 50, 1, glm::vec4(0, 0, 0, 1));
    }

    t.renderText("RESOURCES: " + str(static_cast<int>(_resources)), _window.width() - 500, _window.height() - 20, 1, modeColor);

    ECS::EventManager::update();
    ECS::Manager::update(dt);
    _gameState._bulletParticles.update(dt);
    _spawner.update(dt);

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

  if (key == GLFW_KEY_PERIOD) {
    decrementZoom();
  }
  if (key == GLFW_KEY_COMMA) {
    incrementZoom();
  }

  if (key == GLFW_KEY_F12) {
    restart();
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
  // _reboundViewToWorld();
}


void Game::incrementZoom() {
  tile_view_size = std::min(100, tile_view_size + 5);
  _gameState._view.radius(tile_view_size * tile_size / 2.f * _window.widthScalingFactor(), tile_view_size * tile_size / 2.f);
}

void Game::decrementZoom() {
  tile_view_size = std::max(10, tile_view_size - 5);
  _gameState._view.radius(tile_view_size * tile_size / 2.f * _window.widthScalingFactor(), tile_view_size * tile_size / 2.f);
}
