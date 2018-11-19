#pragma once

// glad must come before glfw3
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <functional>
#include <iostream>

// this plumbing is necessary because
// a lambda that captures can't be used as a function ptr
using KeyCallback = std::function<void(int, int, int, int)>;
static KeyCallback __keyCallback;
static void __keyCallbackWrapper(GLFWwindow* window, int k, int s, int a,
                                 int m) {
  __keyCallback(k, s, a, m);
}

class RenderWindow {
  GLFWwindow* _window;
  int _width;
  int _height;

public:
  RenderWindow(const char* name) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto* monitor = glfwGetPrimaryMonitor();
    auto* vm = glfwGetVideoMode(monitor);
    _width = vm->width;
    _height = vm->height;

    // std::cout << _width << ", " << _height << std::endl;
    // glfwTerminate();
    // exit(0);

    _window = glfwCreateWindow(_width, _height, name, monitor, NULL);
    if (_window == NULL) {
      std::cout << "Failed to createe GLFW window" << std::endl;
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
};

class TriangleShape {
  std::array<float, 18> _vertices;
public:
  GLuint VAO;

  TriangleShape(std::array<float, 18> vertices) {
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  }

  void draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
};