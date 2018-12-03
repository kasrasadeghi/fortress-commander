#pragma once

// glad must come before glfw3
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "View.h"

#include <glm/gtc/matrix_transform.hpp>

#include <functional>
#include <iostream>

// this plumbing is necessary because
// a lambda that captures can't be used as a function ptr
using KeyCallback = std::function<void(int, int, int, int)>;
static KeyCallback __keyCallback;
static void __keyCallbackWrapper(GLFWwindow* window, int k, int s, int a, int m) {
  __keyCallback(k, s, a, m);
}

using MouseCallback = std::function<void(int, int, int)>;
static MouseCallback __mouseCallback;
static void __mouseCallbackWrapper(GLFWwindow* window, int b, int a, int m) {
  __mouseCallback(b, a, m);
}

using CursorCallback = std::function<void(double, double)>;
static CursorCallback __cursorCallback;
static void __cursorCallbackWrapper(GLFWwindow* window, double x, double y) {
  __cursorCallback(x, y);
}

class RenderWindow {
  GLFWwindow* _window;

public:
  int _width;
  int _height;
  View _default;

  RenderWindow(const char* name) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* monitor = glfwGetPrimaryMonitor();
    auto* vm = glfwGetVideoMode(monitor);
    _width = vm->width;
    _height = vm->height;

    _window = glfwCreateWindow(_width, _height, name, monitor, NULL);
    if (_window == NULL) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      exit(-1);
    }
    glfwMakeContextCurrent(_window);

    // load opengl
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      glfwTerminate();
      exit(-1);
    }
    glfwSetFramebufferSizeCallback(_window, frameBufferSizeCallback);
    glViewport(0, 0, _width, _height);

    float height = static_cast<float>(_height);
    float width = static_cast<float>(_width);

    _default.center(width / 2, height / 2).radius(width / 2, height / 2);
  }

  ~RenderWindow() { glfwTerminate(); }

  // clang-format off
  GLFWwindow* window() { return _window; }
  int height()         { return _height; }
  int width()          { return _width;  }

  bool isOpen()      { return not glfwWindowShouldClose(_window); }
  void swapBuffers() { glfwSwapBuffers(_window); }
  void close()       { glfwSetWindowShouldClose(_window, true); }

  View& defaultView()        { return _default; }
  float widthScalingFactor() { return 1.f * _width / _height; }
  // clang-format on

  /////////////////// Callbacks and Input ///////////////

  int getKey(int key_code) {
    return glfwGetKey(_window, key_code);
  }

  glm::vec2 mousePos() {
    // NOTE: this mouse position is "relative to the client area of the window"
    // according to glfw
    double xpos, ypos;
    glfwGetCursorPos(_window, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
  }

  static void frameBufferSizeCallback(GLFWwindow* w, int width, int height) {
    glViewport(0, 0, width, height);
  }

  void setKeyCallback(KeyCallback keyCallback) {
    __keyCallback = keyCallback;
    glfwSetKeyCallback(_window, __keyCallbackWrapper);
  }
  void setMouseCallback(MouseCallback mouseCallback) {
    __mouseCallback = mouseCallback;
    glfwSetMouseButtonCallback(_window, __mouseCallbackWrapper);
  }
  void setCursorCallback(CursorCallback cursorCallback) {
    __cursorCallback = cursorCallback;
    glfwSetCursorPosCallback(_window, __cursorCallbackWrapper);
  }
};