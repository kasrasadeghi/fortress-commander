#pragma once

#include "ECS/Event.h"

struct MouseMoveEvent {
  MouseMoveEvent(float x, float y) : x(x), y(y) {}
  float x, y;
};

struct MouseDownEvent {
  MouseDownEvent(int button, float x, float y) : button(button), x(x), y(y) {}
  int button;
  float x, y;
};

struct MouseUpEvent {
  MouseUpEvent(int button, float x, float y) : button(button), x(x), y(y) {}
  int button;
  float x, y;
};

struct KeyEvent {
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
