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

TEST(Shape, view) {
  auto view = View().center(100.f, 100.f).radius(20.f, 20.f);
  // sfml default view where 0,0 is top left
  auto view_check = glm::ortho(80.f, 120.f, 120.f, 80.f, -1.f, 1.f);
  EXPECT_EQ(view.proj(), view_check);
}

TEST(Shape, pi) {
  using namespace std;
  cout << glm::pi<float>() << endl;
  cout << glm::half_pi<float>() << endl;
}

TEST(Shape, circle) {
  using namespace std;
  uint size_count = 4;
  for (uint i = 0; i < size_count; ++i) {
    float angle = i * 2 * glm::pi<float>() / size_count;
    float x = cos(angle);
    float y = sin(angle);
    cout << angle << ": " << x << ", " << y << endl;
  }
}