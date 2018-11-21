#pragma once

// glad must come before glfw3
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <functional>


// this plumbing is necessary because
// a lambda that captures can't be used as a function ptr
using KeyCallback = std::function<void(int, int, int, int)>;
static KeyCallback __keyCallback;
static void __keyCallbackWrapper(GLFWwindow* window, int k, int s, int a, int m) {
  __keyCallback(k, s, a, m);
}

class RenderWindow {
  GLFWwindow* _window;

public:
  int _width;
  int _height;
  
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
  }

  bool isOpen() { return not glfwWindowShouldClose(_window); }
  void swapBuffers() { glfwSwapBuffers(_window); }

  int getKey(int key_code) { return glfwGetKey(_window, key_code); }
  void close() { glfwSetWindowShouldClose(_window, true); }

  static void frameBufferSizeCallback(GLFWwindow* w, int width, int height) {
    glViewport(0, 0, width, height);
  }

  void setKeyCallback(KeyCallback keyCallback) {
    __keyCallback = keyCallback;
    glfwSetKeyCallback(_window, __keyCallbackWrapper);
  }

  glm::mat4 getDefaultView() {
    return glm::ortho(0.f, static_cast<float>(_width), static_cast<float>(_height), 0.f, -1.f, 1.f);
  }
};