#pragma once

#include "ECS/Event.h"

struct MouseMoveEvent : public ECS::BaseEvent {
  MouseMoveEvent(float x, float y) : x(x), y(y) {}
  float x, y;
};

struct MouseDownEvent : public ECS::BaseEvent {
  MouseDownEvent(int button, float x, float y) : button(button), x(x), y(y) {}
  int button;
  float x, y;
};

struct MouseUpEvent : public ECS::BaseEvent {
  MouseUpEvent(int button, float x, float y) : button(button), x(x), y(y) {}
  int button;
  float x, y;
};

struct KeyEvent : public ECS::BaseEvent {
  KeyEvent(int key, int action) : key(key), action(action) {}
  int key;
  int action;
};

struct KeyDownEvent : public KeyEvent {
  KeyDownEvent(int keycode) : KeyEvent(keycode, GLFW_PRESS) {}
};

struct KeyUpEvent : public KeyEvent {
  KeyUpEvent(int keycode) : KeyEvent(keycode, GLFW_RELEASE) {}
};