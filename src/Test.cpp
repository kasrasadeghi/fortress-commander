#include <gtest/gtest.h>

#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "Game.h"
#include "Graphics.h"
#include "Path.h"
#include "RegionGenerator.h"
#include "World.h"
#include <iostream>

// Game g; // sets up opengl

// TEST(Construction, Shader) {
//   const Shader _shader{"triangle"};
// }

// TEST(Shape, view) {
//   auto view = View().center(100.f, 100.f).radius(20.f, 20.f);
//   // sfml default view where 0,0 is top left
//   auto view_check = glm::ortho(80.f, 120.f, 120.f, 80.f, -1.f, 1.f);
//   EXPECT_EQ(view.proj(), view_check);
// }

// TEST(Shape, pi) {
//   using namespace std;
//   cout << glm::pi<float>() << endl;
//   cout << glm::half_pi<float>() << endl;
// }

// TEST(Shape, circle) {
//   using namespace std;
//   uint size_count = 4;
//   for (uint i = 0; i < size_count; ++i) {
//     float angle = i * 2 * glm::pi<float>() / size_count;
//     float x = cos(angle);
//     float y = sin(angle);
//     cout << angle << ": " << x << ", " << y << endl;
//   }
// }

TEST(Pathing, findPathSpeed) {
  Region region{{world_size, std::vector<Tile>(world_size, Tile::GRASS)}};
  for (int i = 0; i < 100; ++i) {
    findPath(region, {0, 0}, {world_size - 1, world_size - 1});
  }
}

TEST(Perlin, rangeTest) {
  PerlinNoise p(0);
  double min = std::numeric_limits<double>::max();
  double max = std::numeric_limits<double>::lowest();

  std::mt19937 mt(0);
  std::uniform_real_distribution<double> dist{-10, 10};
  for (int i = 0; i < 100000; ++i) {
    double f = p(dist(mt), dist(mt));
    min = std::min(min, f);
    max = std::max(max, f);
  }

  std::cout << min << ", " << max << std::endl;
}

// TEST(Pathing, constructor) {
//   std::array<int, 100> a;
//   a.fill(7);
// for (int i = 0; i < 100; ++i) {
//   std::cout << a[i] << std::endl;
// }
// }