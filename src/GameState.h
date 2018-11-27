#pragma once

#include "Graphics.h"
enum class ControlMode { NONE, BUILD, UNIT, TERRAIN };

/**
 * @brief Encapsulates state shared between Game and its Systems
 */
struct GameState {
  RenderWindow& _window;
  ControlMode _mode = ControlMode::NONE;
  View _view;

  GameState(RenderWindow& window);
};