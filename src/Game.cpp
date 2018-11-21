#include "Game.h"
#include "Graphics.h"
// #include "Config.h"
// #include "Unit.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
// #include <glm/glm.hpp>
// #include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

Game::Game()
    : //_font(), _world(World::world_size),
      //   _view(sf::Vector2f((view_size) / 2.f * widthScalingFactor(),
      //                      (view_size) / 2.f),
      //         sf::Vector2f(view_size * widthScalingFactor(), view_size)),
      _window("Fortress Commander") {
  _window.setKeyCallback([this](auto&&... args) { keyCallback(args...); });
  // _window.setView(_view);
  // _clock.restart();
  // // _window.setFramerateLimit(60);
  // auto v = sf::VideoMode::getDesktopMode();
  // std::cout << v.width << ", " << v.height << std::endl;

  // if (!_font.loadFromFile("arial.ttf")) { exit(1); }
}

void Game::loop() {

  glm::vec2 position(90, 90);
  glm::vec2 size(20, 20);
  GLfloat rotate(0);

  glm::mat4 m(1);
  m = glm::translate(m, glm::vec3(position, 0.f));

  m = glm::translate(m, glm::vec3(position[0] *  .5f, position[1] *  .5f, 0.f));
  m = glm::rotate(m, rotate, glm::vec3(0.f, 0.f, 1.f));
  m = glm::translate(m, glm::vec3(position[0] * -.5f, position[1] * -.5f, 0.f));

  m = glm::scale(m, glm::vec3(size, 1.f));

  glm::vec2 center(100.f, 100.f);
  glm::vec2 radius(20.f, 20.f);

  // glm::mat4 proj = glm::ortho(0.f, 3440.f, 1440.f, 0.f, -1.f, 1.f);
  glm::mat4 proj = glm::ortho(center[0] - radius[0], center[0] + radius[0], center[1] - radius[1], center[1] + radius[1], -1.f, 1.f);
  
  Shader shader {"shaders/view_triangle.vs", "shaders/view_triangle.fs"};
  
  GLuint quadVAO;

  GLfloat vertices[] = { 
    // Pos     
    1.f, 1.f,
    1.f, 0.f,
    0.f, 1.f,
    0.f, 0.f,
  };

  GLuint VBO;
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s),
  // and then configure vertex attributes(s).
  glBindVertexArray(quadVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);


  while (_window.isOpen()) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    shader.setMat4("projection", proj);
    shader.setMat4("model", m);
    shader.setVec3("color", 1.f, .7f, 0.f);


    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
    glBindVertexArray(0);

    // for (Unit& u : _world._units) { u.update(dt); }

    // _window.setView(_view);
    // _window.clear();

    // _window.draw(_world);
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

// void Game::handleViewInput(const sf::Time& dt) {
//   constexpr float speed = 20 * tile_size;
//   float d = (speed * dt).asSeconds();

//   _mouseViewMove(d);

//   _keyboardViewMove(d);

//   _reboundViewToWorld();
// }
