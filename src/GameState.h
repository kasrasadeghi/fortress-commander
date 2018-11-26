#pragma once

#include <SFML/Graphics.hpp>

enum class ControlMode { NONE, BUILD, UNIT, TERRAIN };

/**
 * @brief Encapsulates state shared between Game and its Systems
 */
struct GameState {
    sf::RenderWindow& _window;
    ControlMode _mode = ControlMode::NONE;

    GameState(sf::RenderWindow& window) : _window(window) {}
};