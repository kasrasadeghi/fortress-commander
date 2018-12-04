#include <glad/glad.h>

namespace GL {

struct VertexBuffer {
  uint VBO;

  explicit VertexBuffer() {
    glGenBuffers(1, &VBO); 
  }

  ~VertexBuffer() {
    glDeleteBuffers(1, &VBO);
  }

  void bind() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
  }

  static void unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  template <typename T>
  void bindData(const std::vector<T>& vertices) {
    // assert: sum(attribute sizes) * sizeof(float) == sizeof(T)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
};

struct VertexArray {
private:
public:
  uint attribCounter = 0;

  VertexBuffer VB; // vertex buffer
  VertexBuffer IB; // instance buffer
  uint VAO;

  VertexArray() {
    glGenVertexArrays(1, &VAO);
  }

  ~VertexArray() {
    glDeleteVertexArrays(1, &VAO);
  }

  void bind() {
    glBindVertexArray(VAO);
  }

  void unbind() {
    glBindVertexArray(0);
  }

  /**
   * Takes a list of attribute sizes and creates a buffer with that layout.
   * 
   * @returns the vertex buffer that was created so that you can load data into it.
   */
  VertexBuffer& addBuffer(std::initializer_list<uint> sizes, bool instanced = false) {
    glBindVertexArray(VAO);

    if (instanced) {
      glBindBuffer(GL_ARRAY_BUFFER, IB.VBO);
    } else {
      glBindBuffer(GL_ARRAY_BUFFER, VB.VBO);
    }

    size_t dataOffset = 0;

    for (uint size : sizes) {
      glEnableVertexAttribArray(attribCounter);
      glVertexAttribPointer(attribCounter, size, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)dataOffset);
      if (instanced) { glVertexAttribDivisor(attribCounter, 1); }
      attribCounter++;
      dataOffset += size * sizeof(float);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if (instanced) {
      return IB;
    } else {
      return VB;
    }
  }
};
}
