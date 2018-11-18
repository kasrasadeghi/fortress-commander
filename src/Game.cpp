#include "Game.h"
#include "Unit.h"

#include <SFML/Graphics.hpp>
#include <stdio.h>

#include <sstream>

Game::Game()
    : _font(), _world(World::world_size),
      _view(sf::Vector2f((view_size * World::tile_size) / 2.f *
                             widthScalingFactor(),
                         (view_size * World::tile_size) / 2.f),
            sf::Vector2f(view_size * World::tile_size * widthScalingFactor(),
                         view_size * World::tile_size)),
      _window(sf::VideoMode::getFullscreenModes()[0], "Fortress Commander",
              sf::Style::Fullscreen) {
  _window.setView(_view);
  _clock.restart();
  _window.setFramerateLimit(60);

  if (!_font.loadFromFile("arial.ttf")) {
    exit(1);
  }
}

void Game::loop() {
  while (_window.isOpen()) {
    auto dt = _clock.getElapsedTime();
    auto framerate = (1 / dt.asSeconds());
    _clock.restart();

    std::stringstream ss;
    ss << framerate;
    sf::Text t;
    t.setFont(_font);
    t.setString(ss.str());
    t.setCharacterSize(24);
    t.setFillColor(sf::Color::Black);

    sf::Event event;
    while (_window.pollEvent(event)) {
      handleEvent(event);
    }

    handleViewInput(dt);

    _window.setView(_view);
    _window.clear();

    _window.draw(_world);
    if (_mode == ControlMode::BUILD) {
      _buildManager.setMouseTile(getMouseTile());
      _window.draw(_buildManager);
    }

    _window.setView(_window.getDefaultView());
    _window.draw(t);

    _window.display();
  }
}

void Game::handleEvent(const sf::Event& event) {
  if (event.type == sf::Event::Closed) {
    _window.close();
  }
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Q) {
      _window.close();
    }
    if (event.key.code == sf::Keyboard::B) {
      _mode = ControlMode::BUILD;
    }
    if (event.key.code == sf::Keyboard::U) {
      _mode = ControlMode::UNIT;
    }
    if (event.key.code == sf::Keyboard::Escape) {
      _mode = ControlMode::NONE;
    }
  }

  if (_mode == ControlMode::BUILD) {
    // build structures instead of editing the world
    if (event.type == sf::Event::MouseButtonPressed) {
      _paint = _world.flipCell(getMouseTile());
    }
    if (event.type == sf::Event::MouseMoved &&
        sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      _world.setCell(getMouseTile(), _paint);
    }
  }
  if (_mode == ControlMode::UNIT) {
    if (event.type == sf::Event::MouseButtonPressed) {
      // check if the add unit is in bounds
      _world.addUnit(Unit(getMouseCoords()));
    }
  }
}

void Game::handleViewInput(const sf::Time& dt) {
  constexpr float speed = 20 * World::tile_size;
  float d = (speed * dt).asSeconds();

  // mouse
  constexpr int margin = 20;
  auto pos = sf::Mouse::getPosition();
  if (pos.x < margin) {
    _view.move(-d, 0);
  }
  if (pos.y < margin) {
    _view.move(0, -d);
  }

  if (pos.x > _window.getSize().x - margin) {
    _view.move(d, 0);
  }
  if (pos.y > _window.getSize().y - margin) {
    _view.move(0, d);
  }

  // keyboard
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    _view.move(0, -d);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    _view.move(0, d);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    _view.move(-d, 0);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    _view.move(d, 0);
  }
  if (_mode == ControlMode::BUILD) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      _world.setCell(getMouseTile(), _paint);
    }
  }

  // lock view to world
  auto topLeft = _view.getCenter() - (_view.getSize() / 2.f);
  auto bottomRight = _view.getCenter() + (_view.getSize() / 2.f);

  const auto viewRadius = (view_size * World::tile_size) / 2.f;
  const auto worldBorder = World::world_size * World::tile_size;

  if (topLeft.x < 0) {
    _view.setCenter(viewRadius * widthScalingFactor(), _view.getCenter().y);
  }
  if (topLeft.y < 0) {
    _view.setCenter(_view.getCenter().x, viewRadius);
  }
  if (bottomRight.x > worldBorder) {
    _view.setCenter(worldBorder - (viewRadius * widthScalingFactor()),
                    _view.getCenter().y);
  }
  if (bottomRight.y > worldBorder) {
    _view.setCenter(_view.getCenter().x, worldBorder - viewRadius);
  }
}
