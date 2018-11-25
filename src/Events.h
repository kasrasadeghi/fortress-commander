#pragma once

#include "ECS/Event.h"

struct MouseMoveEvent : public ECS::BaseEvent {
  MouseMoveEvent(float ax, float ay) : x(ax), y(ay) {}
  ~MouseMoveEvent() = default;

  float x, y;
};

struct MouseDownEvent : public ECS::BaseEvent {
  MouseDownEvent(float ax, float ay) : x(ax), y(ay) {}
  ~MouseDownEvent() = default;

  float x, y;
};

struct MouseUpEvent : public ECS::BaseEvent {
  MouseUpEvent() {}
  ~MouseUpEvent() = default;
};
