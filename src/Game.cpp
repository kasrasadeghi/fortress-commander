#include "Game.h"
#include "Config.h"
#include "Graphics.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <sstream>
#include <vector>

Game::Game() : _world(World::world_size), _window("Fortress Commander") {
  _window.setKeyCallback([this](auto&&... args) { keyCallback(args...); });
  _window.setMouseCallback([this](auto&&... args) { mouseCallback(args...); });
  // glfwSwapInterval(1);
  _view.center(view_size / 2.f * _window.widthScalingFactor(), view_size / 2.f)
      .radius(view_size / 2.f * _window.widthScalingFactor(), view_size / 2.f);
  // if (!_font.loadFromFile("arial.ttf")) { exit(1); }
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

    for (Unit& u : _world._units) {
      u.update(dt);
    }

    handleTick(dt);

    _world.draw(_view);

    t.renderText(str(1.f / dt), 100, 50, 1, glm::vec3(0, 0, 0));

    if (_mode == ControlMode::BUILD || _mode == ControlMode::TERRAIN) {
      World::tileHolo(_view, getMouseTile());
    }
    if (_mode == ControlMode::UNIT) {
      Unit::holo(_view, getMouseCoords());
    }

    _window.swapBuffers();
    glfwPollEvents();
  }
}

void Game::keyCallback(int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    _window.close();
  }

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

void Game::mouseCallback(int button, int action, int mods) {
  // if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
  //   auto p = getMouseTile();
  //   std::cout << p.x << ", " << p.y << std::endl;
  // }
  if (action == GLFW_PRESS) {
    switch (_mode) {
    case ControlMode::NONE:
      if (_world._units.size()) _world._units[0].pathTo(getMouseCoords());
      break;
    case ControlMode::BUILD:
      // TODO:_world.addStructure(getMouseTile());
      break;
    case ControlMode::TERRAIN:
      _paint = _world.flipCell(getMouseTile());
      break;
    case ControlMode::UNIT:
      // TODO: check if the add unit is in bounds
      _world._units.push_back(Unit(getMouseCoords()));
      break;
    }
  }

  //   if (_mode == ControlMode::TERRAIN && event.type == sf::Event::MouseMoved &&
  //       sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
  //     _world.setCell(getMouseTile(), _paint);
  //   }
}

void Game::handleTick(float dt) {
  constexpr float speed = 20 * tile_size;
  float d = speed * dt;

  _mouseViewMove(d);

  _keyboardViewMove(d);

  // lock view to world by rebounding
  _reboundViewToWorld();
}
