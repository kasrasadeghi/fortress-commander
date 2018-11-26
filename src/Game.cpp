#include "Game.h"

#include <sstream>
#include <stdio.h>

#include "Events.h"
#include "Systems.h"

#include <SFML/Graphics.hpp>

Game::Game()
    : _font(), _world(World::world_size),
      _view(sf::Vector2f((view_size * World::tile_size) / 2.f *
                             widthScalingFactor(),
                         (view_size * World::tile_size) / 2.f),
            sf::Vector2f(view_size * World::tile_size * widthScalingFactor(),
                         view_size * World::tile_size)),
      _window(sf::VideoMode::getFullscreenModes()[0], "Fortress Commander",
              sf::Style::Fullscreen),
      _manager(), _eventManager(std::allocator<void>()),
      _gameState(_window) {
  _window.setView(_view);
  _clock.restart();
  _window.setFramerateLimit(60);

  if (!_font.loadFromFile("arial.ttf")) { exit(1); }

  _manager.createComponentStore<TransformComponent>();
  _manager.createComponentStore<MotionComponent>();
  _manager.createComponentStore<SelectableComponent>();
  _manager.createComponentStore<CommandableComponent>();

  _moveSystem = new MoveSystem(_manager, _eventManager, _gameState);
  _manager.addSystem(ECS::System::Ptr(_moveSystem));

  _unitSelectSystem = new UnitSelectSystem(_manager, _eventManager, _gameState);
  _manager.addSystem(ECS::System::Ptr(_unitSelectSystem));

  _unitCommandSystem = new UnitCommandSystem(_manager, _eventManager, _gameState);
  _manager.addSystem(ECS::System::Ptr(_unitCommandSystem));

  _eventManager.connect<KeyDownEvent>(this);
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

    _window.setView(_view);
    _window.clear();

    _window.draw(_world);
    if (_gameState._mode == ControlMode::BUILD || _gameState._mode == ControlMode::TERRAIN) {
      const auto currTile = getMouseTile();
      sf::RectangleShape r(sf::Vector2f(World::tile_size, World::tile_size));
      r.setPosition(currTile.x * World::tile_size,
                    currTile.y * World::tile_size);
      r.setFillColor(sf::Color(255, 200, 200, 200));
      _window.draw(r);
    }

    if (_gameState._mode == ControlMode::UNIT) {
      constexpr auto radius = Unit::unit_size * World::tile_size;
      const auto curr = getMouseCoords();
      sf::CircleShape r(Unit::unit_size * World::tile_size);
      r.setPosition(curr.x - radius, curr.y - radius);
      r.setFillColor(sf::Color(255, 200, 200, 150));
      _window.draw(r);
    }

    _eventManager.update();
    _manager.update(dt.asSeconds());

    _window.setView(_window.getDefaultView());
    _window.draw(t);

    _window.display();
  }
}

void Game::receive(ECS::EventManager* mgr, const KeyDownEvent& e) {
  if (e.keycode == sf::Keyboard::Q) _window.close();
  if (e.keycode == sf::Keyboard::B) _gameState._mode = ControlMode::BUILD;
  if (e.keycode == sf::Keyboard::U) _gameState._mode = ControlMode::UNIT;
  if (e.keycode == sf::Keyboard::T) _gameState._mode = ControlMode::TERRAIN;
  if (e.keycode == sf::Keyboard::Escape) _gameState._mode = ControlMode::NONE;
}

void Game::handleEvent(const sf::Event& event) {
  if (event.type == sf::Event::Closed) { _window.close(); }
  else if (event.type == sf::Event::KeyPressed) {
    _eventManager.event(new KeyDownEvent(event.key.code));
  }
  else if (event.type == sf::Event::KeyReleased) {
    _eventManager.event(new KeyUpEvent(event.key.code));
  }
  else if (event.type == sf::Event::MouseButtonPressed) {
    _eventManager.event(new MouseDownEvent(event.mouseButton.button, getMouseCoords().x, getMouseCoords().y));
  }
  else if (event.type == sf::Event::MouseButtonReleased) {
    _eventManager.event(new MouseUpEvent(event.mouseButton.button, getMouseCoords().x, getMouseCoords().y));
  }
  else if (event.type == sf::Event::MouseMoved) {
    _eventManager.event(new MouseMoveEvent(getMouseCoords().x, getMouseCoords().y));
  }

  if (_gameState._mode == ControlMode::BUILD) {
    //_world.addStructure(getMouseTile());
  }
  if (_gameState._mode == ControlMode::TERRAIN) {
    if (event.type == sf::Event::MouseButtonPressed) {
      _paint = _world.flipCell(getMouseTile());
    }
    if (event.type == sf::Event::MouseMoved &&
        sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      _world.setCell(getMouseTile(), _paint);
    }
  }
  if (_gameState._mode == ControlMode::UNIT) {
    if (event.type == sf::Event::MouseButtonPressed) {
      // check if the add unit is in bounds
      _world._units.push_back(Unit(getMouseCoords(), &_manager));
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
  if (_gameState._mode == ControlMode::TERRAIN) {
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
