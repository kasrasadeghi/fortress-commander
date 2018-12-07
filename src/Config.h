#pragma once

using HealthValue = short;
using StrengthValue = short;
using ResourceType = int;

// many rendered things are used as k * tile_size, like unit_size, view_size
constexpr float tile_size = 1.f;
constexpr int world_size = 100;

constexpr ResourceType sell_ratio = 2;
constexpr ResourceType init_resource_bal = 1000;

constexpr float muzzleFlashTime = 0.15f;
