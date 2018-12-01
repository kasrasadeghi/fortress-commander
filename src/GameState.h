#pragma once

#include "Graphics.h"

#include "Enemy.h"

enum class ControlMode { NONE, BUILD, SELL, UNIT, TERRAIN };

class Unit;
class Enemy;

/**
 * @brief Encapsulates state shared between Game and its Systems
 */
struct GameState {
  RenderWindow& _window;
  ControlMode _mode = ControlMode::NONE;
  View _view;

  std::vector<Unit>& units;
  std::vector<Enemy>& enemies;

  GameState(RenderWindow& window, std::vector<Unit>& units, std::vector<Enemy>& enemies);
};
