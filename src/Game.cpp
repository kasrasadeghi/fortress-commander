#include "Game.h"
#include "Components.h"
#include "Systems.h"
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
              sf::Style::Fullscreen),
      _manager() {
  _window.setView(_view);
  _clock.restart();
  _window.setFramerateLimit(60);

  if (!_font.loadFromFile("arial.ttf")) { exit(1); }

  ECS::Entity entity = _manager.createEntity();
  std::cout << entity << std::endl;

  _manager.createComponentStore<TransformComponent>();

  _manager.addComponent<TransformComponent>(
      entity, TransformComponent(sf::Vector2f(1.f, 1.f), 0.f));

  MoveSystem moveSystem(_manager);
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
    while (_window.pollEvent(event)) { handleEvent(event); }

    handleViewInput(dt);
    for (Unit& u : _world._units) { u.update(dt); }

    _window.setView(_view);
    _window.clear();

    _window.draw(_world);
    if (_mode == ControlMode::BUILD || _mode == ControlMode::TERRAIN) {
      const auto currTile = getMouseTile();
      sf::RectangleShape r(sf::Vector2f(World::tile_size, World::tile_size));
      r.setPosition(currTile.x * World::tile_size,
                    currTile.y * World::tile_size);
      r.setFillColor(sf::Color(255, 200, 200, 200));
      _window.draw(r);
    }

    if (_mode == ControlMode::UNIT) {
      constexpr auto radius = Unit::unit_size * World::tile_size;
      const auto curr = getMouseCoords();
      sf::CircleShape r(Unit::unit_size * World::tile_size);
      r.setPosition(curr.x - radius, curr.y - radius);
      r.setFillColor(sf::Color(255, 200, 200, 150));
      _window.draw(r);
    }

    _window.setView(_window.getDefaultView());
    _window.draw(t);

    _window.display();
  }
}

void Game::handleEvent(const sf::Event& event) {
  if (event.type == sf::Event::Closed) { _window.close(); }
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Q) { _window.close(); }
    if (event.key.code == sf::Keyboard::B) { _mode = ControlMode::BUILD; }
    if (event.key.code == sf::Keyboard::U) { _mode = ControlMode::UNIT; }
    if (event.key.code == sf::Keyboard::T) { _mode = ControlMode::TERRAIN; }
    if (event.key.code == sf::Keyboard::Escape) { _mode = ControlMode::NONE; }
  }

  if (_mode == ControlMode::NONE) {
    if (event.type == sf::Event::MouseButtonPressed) {
      if (_world._units.size()) _world._units[0].pathTo(getMouseCoords());
    }
  }
  if (_mode == ControlMode::BUILD) {
    //_world.addStructure(getMouseTile());
  }
  if (_mode == ControlMode::TERRAIN) {
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
      _world._units.push_back(Unit(getMouseCoords()));
    }
  }
}

void Game::handleViewInput(const sf::Time& dt) {
  constexpr float speed = 20 * World::tile_size;
  float d = (speed * dt).asSeconds();

  // mouse
  constexpr int margin = 20;
  auto pos = sf::Mouse::getPosition();
  if (pos.x < margin) { _view.move(-d, 0); }
  if (pos.y < margin) { _view.move(0, -d); }

  if (pos.x > _window.getSize().x - margin) { _view.move(d, 0); }
  if (pos.y > _window.getSize().y - margin) { _view.move(0, d); }

  // keyboard
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { _view.move(0, -d); }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { _view.move(0, d); }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { _view.move(-d, 0); }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { _view.move(d, 0); }
  if (_mode == ControlMode::TERRAIN) {
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
  if (topLeft.y < 0) { _view.setCenter(_view.getCenter().x, viewRadius); }
  if (bottomRight.x > worldBorder) {
    _view.setCenter(worldBorder - (viewRadius * widthScalingFactor()),
                    _view.getCenter().y);
  }
  if (bottomRight.y > worldBorder) {
    _view.setCenter(_view.getCenter().x, worldBorder - viewRadius);
  }
}
