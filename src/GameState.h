#pragma once

enum class ControlMode { NONE, BUILD, UNIT, TERRAIN };

/**
 * @brief Encapsulates state shared between Game and its Systems
 */
struct GameState {
    ControlMode _mode = ControlMode::NONE;
};