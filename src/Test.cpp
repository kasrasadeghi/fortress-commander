#include <gtest/gtest.h>

#include <glm/gtc/matrix_transform.hpp>


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "Game.h"
#include "Graphics.h"

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
  GLfloat rotate(0);

  EXPECT_EQ(r.position(), position);
  EXPECT_EQ(r.size(), size);
  EXPECT_EQ(r.rotate(), rotate);

  // TODO make _position and other shape things private and use a viewer friend with a #define or
  // something
}

TEST(Shape, model) {
  RectangleShape r;
  r.position(90, 90).size(20, 20);

  glm::vec2 position(90, 90);
  glm::vec2 size(20, 20);
  GLfloat rotate(0);

  glm::mat4 m(1);
  m = glm::translate(m, glm::vec3(position, 0.f));

  m = glm::translate(m, glm::vec3(position[0] * .5f, position[1] * .5f, 0.f));
  m = glm::rotate(m, rotate, glm::vec3(0.f, 0.f, 1.f));
  m = glm::translate(m, glm::vec3(position[0] * -.5f, position[1] * -.5f, 0.f));

  m = glm::scale(m, glm::vec3(size, 1.f));

  r.computeModel();
  EXPECT_EQ(m, r.model());
}

TEST(Shape, view) {
  auto view = View().center(100.f, 100.f).radius(20.f, 20.f);
  // sfml default view where 0,0 is top left
  auto view_check = glm::ortho(80.f, 120.f, 120.f, 80.f, -1.f, 1.f);
  EXPECT_EQ(glm::to_string(view.proj()), glm::to_string(view_check));
}

TEST(What, what) {
  glm::vec3 v(10.f, 20.f, 0.f);
  glm::mat4 m(2.f);
  m = glm::translate(m, v);
  // m = glm::translate(m, v);

  // std::cout << glm::to_string(result) << std::endl;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      std::cout << m[j][i] << "  ";
    }
    std::cout << std::endl;
  }
}