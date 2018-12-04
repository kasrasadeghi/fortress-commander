#include <glad/glad.h>

namespace GL {

struct VertexBuffer {
  uint VBO;

  template <typename T>
  explicit VertexBuffer(const std::vector<T>& vertices) {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  ~VertexBuffer() {
    glDeleteBuffers(1, &VBO);
  }
};

struct VertexArray {
private:
public:
  uint attribCounter = 0;

  std::vector<VertexBuffer> _buffers;
  uint VAO;

  VertexArray() {
    glGenVertexArrays(1, &VAO);
  }

  ~VertexArray() {
    glDeleteVertexArrays(1, &VAO);
  }

  template <typename T>
  void addBuffer(const std::vector<T>& vertices, std::initializer_list<uint> sizes) {
    // assert: sum(sizes) * sizeof(float) == sizeof(T)
    _buffers.emplace_back(vertices);
    glBindBuffer(GL_ARRAY_BUFFER, _buffers.back().VBO);

    size_t dataOffset = 0;

    glBindVertexArray(VAO);
    for (uint size : sizes) {
      glEnableVertexAttribArray(attribCounter);
      glVertexAttribPointer(attribCounter, size, GL_FLOAT, GL_FALSE, sizeof(T), (void*)dataOffset);
      dataOffset += size * sizeof(float);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
};
}
