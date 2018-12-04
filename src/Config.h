#pragma once

using HealthValue = short;
using StrengthValue = short;

// many rendered things are used as k * tile_size, like unit_size, view_size
constexpr float tile_size = 1.f;
constexpr int world_size = 100;

constexpr float muzzleFlashTime = 0.15f;
