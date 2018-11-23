#include "Game.h"
#include "Graphics.h"
#include "Config.h"
// #include "Unit.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
// #include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

Game::Game()
    : _world(World::world_size),
      _window("Fortress Commander") {
  _window.setKeyCallback([this](auto&&... args) { keyCallback(args...); });
  glfwSwapInterval(1);
  _view
    .center(view_size/2.f * _window.widthScalingFactor(), view_size/2.f)
    .radius(view_size/2.f * _window.widthScalingFactor(), view_size/2.f);
  // if (!_font.loadFromFile("arial.ttf")) { exit(1); }
}

void Game::loop() {
  float last_time = glfwGetTime();

  while (_window.isOpen()) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  
    float dt = glfwGetTime() - last_time;
    last_time = glfwGetTime();

    // for (Unit& u : _world._units) { u.update(dt); }

    handleTick(dt);
    
    // float s = glfwGetTime();
    _world.draw(_view);
    // float e = glfwGetTime();
    printf("%f\n", 1.f/ (dt));

    // if (_mode == ControlMode::BUILD || _mode == ControlMode::TERRAIN) {
    //   _window.draw(World::tileHolo(getMouseTile()));
    // }
    // if (_mode == ControlMode::UNIT) {
    //   _window.draw(Unit::holo(getMouseCoords()));
    // }

    // _window.setView(_window.getDefaultView());
    // _window.draw(makeText(framerate));

    // _window.display();
    _window.swapBuffers();
    glfwPollEvents();
  }
}

void Game::keyCallback(int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { _window.close(); }
}

// void Game::handleEvent(const sf::Event& event) {
//   if (event.type == sf::Event::Closed) { _window.close(); }
//   if (event.type == sf::Event::KeyPressed) {
//     if (event.key.code == sf::Keyboard::Q) { _window.close(); }
//     if (event.key.code == sf::Keyboard::B) { _mode = ControlMode::BUILD; }
//     if (event.key.code == sf::Keyboard::U) { _mode = ControlMode::UNIT; }
//     if (event.key.code == sf::Keyboard::T) { _mode = ControlMode::TERRAIN; }
//     if (event.key.code == sf::Keyboard::Escape) { _mode = ControlMode::NONE;
//     }
//   }

//   if (event.type == sf::Event::MouseButtonPressed) {
//     switch(_mode) {
//       case ControlMode::NONE:
//         if (_world._units.size()) _world._units[0].pathTo(getMouseCoords());
//         break;
//       case ControlMode::BUILD:
//         //_world.addStructure(getMouseTile());
//         break;
//       case ControlMode::TERRAIN:
//         _paint = _world.flipCell(getMouseTile());
//         break;
//       case ControlMode::UNIT:
//         // check if the add unit is in bounds
//         _world._units.push_back(Unit(getMouseCoords()));
//         break;
//     }
//   }

//   if (_mode == ControlMode::TERRAIN && event.type == sf::Event::MouseMoved &&
//       sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
//     _world.setCell(getMouseTile(), _paint);
//   }
// }

void Game::handleTick(float dt) {
  constexpr float speed = 20 * tile_size;
  float d = speed * dt;

  // _mouseViewMove(d);

  _keyboardViewMove(d);

  // _reboundViewToWorld();
}
