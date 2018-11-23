#include <gtest/gtest.h>

#include <glm/gtc/matrix_transform.hpp>


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "Game.h"
#include "Graphics.h"
#include "Graphics/Instanced.h"

Game g; // sets up opengl

TEST(Construction, Shader) {
  const Shader _shader{"triangle"};
}

TEST(Construction, RectangleShape) { RectangleShape r; }

TEST(Shape, RectBuilder_NonCrashing) {
  RectangleShape r;
  r.position(90, 90).size(20, 20);
}

TEST(Shape, RectBuilder_Value) {
  RectangleShape r;
  r.position(90, 90).size(20, 20);

  glm::vec2 position(90, 90);
  glm::vec2 size(20, 20);

  EXPECT_EQ(r.position(), position);
  EXPECT_EQ(r.size(), size);

  // TODO make _position and other shape things private and use a viewer friend with a #define or
  // something
}

TEST(Shape, model) {
  RectangleShape r;
  r.position(90, 90).size(20, 20);

  glm::vec2 position(90, 90);
  glm::vec2 size(20, 20);

  glm::mat4 m(1);
  m = glm::translate(m, glm::vec3(position, 0.f));

  m = glm::scale(m, glm::vec3(size, 1.f));

  r.computeModel();
  EXPECT_EQ(m, r.model());
}

TEST(Shape, view) {
  auto view = View().center(100.f, 100.f).radius(20.f, 20.f);
  // sfml default view where 0,0 is top left
  auto view_check = glm::ortho(80.f, 120.f, 120.f, 80.f, -1.f, 1.f);
  EXPECT_EQ(view.proj(), view_check);
}

TEST(InstancedRectangle, EvensComputation) {
  InstancedRectangle r;
  r.color(.4, .5, .1).size(1, 1);

  std::vector<glm::vec2> offsets;

  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      if ((i + j) % 2)
        offsets.emplace_back(i * tile_size, j * tile_size);
    }
  }

  View v;
  v.center(5, 5).radius(5, 5);

  for (uint i = 0; i < offsets.size(); ++i) {
    glm::vec2 vertex(1.f, -1.f);
    glm::vec2 vertex2(-1.f, 1.f);

    auto size = r.size();
    auto currpos = offsets[i];
    glm::mat4 model(1.f);
    // std::cout << glm::to_string(model) << std::endl;
    model[0][0] = size[0];
    model[1][1] = size[1];

    model[3][0] = currpos[0];
    model[3][1] = currpos[1];
    
    using namespace std;
    using namespace glm;
    cout << currpos[0] << ", " << currpos[1] << " -> ";
    cout << to_string(v.proj() * model * vec4(vertex, 0.0, 1.0)) << "  ";
    cout << to_string(v.proj() * model * vec4(vertex2, 0.0, 1.0)) << endl;
  }  
}