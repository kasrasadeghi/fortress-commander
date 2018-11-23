#include "World.h"
#include "Graphics.h"
#include "Unit.h"
#include "Graphics/Instanced.h"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void mat4_print(glm::mat4 m) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      std::cout << m[j][i] << "  ";
    }
    std::cout << std::endl;
  }
}

void World::_drawRegion(View& view) const {

  InstancedRectangle r;
  r.color(.4, .5, .1).size(1, 1);

  // for (int i = 0; i < world_size; ++i) {
  //   for (int j = 0; j < world_size; ++j) {
  //     if ((i + j) % 2) {
  //       evens.emplace_back(i * tile_size, j * tile_size);
  //     } else {
  //       odds.emplace_back(i * tile_size, j * tile_size);
  //     }
  //   }
  // }
  std::vector<glm::vec2> offsets;

  // for (int i = 0; i < 10; ++i) {
  //   for (int j = 0; j < 10; ++j) {
  //     offsets.emplace_back(i * 0.1, j * 0.1);
  //   }
  // }

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
  // exit(0);
    
    // std::cout << std::boolalpha 
    //   << glm::to_string(model * glm::vec4(vertex, 0.f, 1.f)) << std::endl
    //   << glm::to_string(glm::vec4(vertex + translations[i], 0.f, 1.f)) << std::endl
    //   << (model * glm::vec4(vertex, 0.f, 1.f) == glm::vec4(vertex + translations[i], 0.f, 1.f))
    //   ;

  r.positions(offsets);
  r.draw(view);

  // for (int i = 0; i < world_size; ++i) {
  //   for (int j = 0; j < world_size; ++j) {
  //     RectangleShape r;
  //     r.size(tile_size, tile_size).position(i * tile_size, j * tile_size);

  //     r.color((/* Tile::GRASS == _region[i][j] */ (i + j) % 2) 
  //       ? glm::vec3(60.f / 255.f, 150.f / 255.f, 40.f / 255.f)
  //       : glm::vec3(0, 0, 1));
  //     r.draw(view);
  //   }
  // }
}

// void World::_drawUnits(sf::RenderTarget& window) const {
//   for (const Unit& u : _units) { window.draw(u); }
// }