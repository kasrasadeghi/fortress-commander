1 - load vectors of data into vertex buffers
2 - create a vertex array
3 - bind the vertex buffer to an attribute in some corresponding shader


1 - load vectors of data into vertex buffers
here's an instance of how to do that

creating a single vertex buffer and loading it with a vector/array of vec2s:
```c++
using VertexBuffer = GLuint;
auto createVertexBuffer(const std::vector<glm::vec2>& vertices) -> VertexBuffer {
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return VBO;
}
```

2 - create a vertex array
```c++
using VertexArray = GLuint;
auto createVertexArray() -> VertexArray {
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  return VAO;
}
```

3 - bind the vertex buffer to an attribute in some corresponding shader
```c++
// set a vertex array that we're going to bind buffers and modify attributes for
glBindVertexArray(VAO);

  // bind a single vec2 array
  // vertVBO : VertexBuffer<glm::vec2>
  glBindBuffer(GL_ARRAY_BUFFER, *vertVBO);

    // set attribute
    // set a attribute to the target attribute in the shader, in this case 0
    glEnableVertexAttribArray(0);

    // for attribute 0, there are 2 floats that are not normalized that are separated by the size of 2 floats, at (void*)0 which is the 
    // beginning of the vertex buffer that we've currently bound
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    
  // unbind the vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);  
  
  // instanced example
  glBindBuffer(GL_ARRAY_BUFFER, *posVBO);
    // set attribute 1
    glEnableVertexAttribArray(_1_);
    glVertexAttribPointer(_1_, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glVertexAttribDivisor(_1_, 1); // tell OpenGL to increment what element of this we're using every 1 instance
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // a buffer with more than one attribute per element, a glm::vec<5, float>
  glBindBuffer(GL_ARRAY_BUFFER, *posVBO);
    // set attribute 2
    glEnableVertexAttribArray(_2_);
    //                         v- size                v- how long the total is  v- starting position
    glVertexAttribPointer(_2_, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    
    // set attribute 3
    glEnableVertexAttribArray(_3_);
    glVertexAttribPointer(_3_, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    //                         ^- size                ^- how long the total is   ^- starting position
    // the only bit that changes is the starting position because it occurs after previous attribute in the same buffer
    // therefore each element looks like [ | ][ | | ]
    //                                   ^_2_ ^_3_
  glBindBuffer(GL_ARRAY_BUFFER, 0);

glBindVertexArray(0);
```