#pragma once

#include "Config.h"
#include "GlmHashes.h"
#include "Graphics.h"
#include "Grid.h"
#include "Tile.h"

#include <unordered_set>
#include <vector>

class Region {
  std::vector<std::vector<Tile>> _data;
  Grid<> _structure_pos_set;
  Texture _tex;

public:
  Region(std::vector<std::vector<Tile>> data) : _data(data) {
    for (auto& v : _structure_pos_set) {
      v.fill(0);
    }
  }

  std::vector<Tile>& operator[](size_t i) {
    return _data[i];
  }

  Tile& at(glm::ivec2 p) {
    if (not inBounds({p.x, p.y})) {
      return _data[0][0]; // What else could we return here?
    }

    return _data[p.x][p.y];
  }

  void draw(View& view) const {
    const glm::vec2 offset(-tile_size * 0.5, -tile_size * 0.5);
    RectangleBatch rects;
    TextureBatch b(_tex);

    for (uint i = 0; i < _data.size(); ++i) {
      for (uint j = 0; j < _data[i].size(); ++j) {
        // clang-format off
        if ( // cpu view culling
          (i + 1) * tile_size  < view.left()   ||
          view.right()         < i * tile_size ||
          (j + 1) * tile_size  < view.bottom() || 
          view.top()           < j * tile_size 
        ) continue;
        // clang-format on

        float texi = _data[i][j] == Tile::GRASS ? 1 : 4;
        auto pos = glm::vec2{i * tile_size, j * tile_size} - offset;

        b.add(TextureBatch::Instance{.pos = pos, .size = {tile_size, tile_size}, .texOffset = texi});
      }
    }

    b.update();
    b.view(view);
    b.draw();

    rects.draw(view);
  }

  bool structureAt(glm::ivec2 cell) const;
  void addStructure(glm::ivec2 cell);

  void removeStructure(glm::ivec2 cell);

  bool inBounds(glm::vec2 pos) const;
};
