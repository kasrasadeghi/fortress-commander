#include <Game.h>

#include <SFML/Graphics.hpp>
#include <stdio.h>

Game::Game(): 
    _view (sf::Vector2f(0, 0), sf::Vector2f(10 * tile_size 
         * sf::VideoMode::getFullscreenModes()[0].width / sf::VideoMode::getFullscreenModes()[0].height, 10 * tile_size)),
      _window (sf::VideoMode::getFullscreenModes()[0], "Fortress Commander", sf::Style::Fullscreen) {
    _window.setView(_view);
    _clock.restart();
}

void Game::loop() {
    while (_window.isOpen()) {
        auto t = _clock.getElapsedTime();
        auto framerate = (1 / t.asSeconds());
        _clock.restart();

        sf::Event event;
        while (_window.pollEvent(event)) {
            handleEvent(event);
        }

        handleViewInput(t);

        _window.setView(_view);
        _window.clear();


    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            sf::RectangleShape r(sf::Vector2f(tile_size, tile_size));
            r.setPosition(sf::Vector2f(i * tile_size, j * tile_size));

            if ((i + j) % 2) {
                r.setFillColor(sf::Color(0, 50, 150));
            } else {
                r.setFillColor(sf::Color(0, 150, 0));
            }
            _window.draw(r);
        }
    }

    _window.display();
    sf::sleep(sf::seconds(1.0f/ 60) - _clock.getElapsedTime());
    }
}

void Game::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        _window.close();
    }
    if (event.key.code == sf::Keyboard::Escape) {
        _window.close();
    }
  }

void Game::handleViewInput(const sf::Time& dt) {
    constexpr float speed = 10;
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
}
