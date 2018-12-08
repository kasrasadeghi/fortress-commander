#pragma once

#include "Config.h"
#include "Region.h"

#include "GlmHashes.h"

#include <glm/gtc/matrix_transform.hpp>

#include <random>

class PerlinNoise {
  int _seed;
  std::uniform_int_distribution<int> _hashDist{0, 255};

public:
  PerlinNoise(int seed) : _seed(seed) {}

  double operator()(double x, double y = 0, double z = 0) {
    const int X = static_cast<int>(floor(x)) & 255; // FIND UNIT CUBE THAT
    const int Y = static_cast<int>(floor(y)) & 255; // CONTAINS POINT.
    const int Z = static_cast<int>(floor(z)) & 255;
    x -= floor(x);                // FIND RELATIVE X,Y,Z
    y -= floor(y);                // OF POINT IN CUBE.
    z -= floor(z);
    const double u = fade(x);     // COMPUTE FADE CURVES
    const double v = fade(y);     // FOR EACH OF X,Y,Z.
    const double w = fade(z);

    const int A = hash(X  )+Y, AA = hash(A)+Z, AB = hash(A+1)+Z, // HASH COORDINATES OF
              B = hash(X+1)+Y, BA = hash(B)+Z, BB = hash(B+1)+Z; // THE 8 CUBE CORNERS,

    double f = lerp(w, lerp(v, lerp(u, grad(hash(AA  ), x  , y  , z   ),  // AND ADD
                                       grad(hash(BA  ), x-1, y  , z   )), // BLENDED
                               lerp(u, grad(hash(AB  ), x  , y-1, z   ),  // RESULTS
                                       grad(hash(BB  ), x-1, y-1, z   ))),// FROM  8
                       lerp(v, lerp(u, grad(hash(AA+1), x  , y  , z-1 ),  // CORNERS
                                       grad(hash(BA+1), x-1, y  , z-1 )), // OF CUBE
                               lerp(u, grad(hash(AB+1), x  , y-1, z-1 ),
                                       grad(hash(BB+1), x-1, y-1, z-1 ))));
    f = f / sqrt(3) + 0.5f;  // map from [-sqrt(n)/2, sqrt(n)/2] to [0, 1] where n is dimensionality
    return std::max(0., std::min(1., f));
  }

  int hash(int x) {
    std::mt19937 twister(_seed + x);
    return _hashDist(twister);
  }

  static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
  static double lerp(double t, double a, double b) { return a + t * (b - a); }
  static double grad(int hash, double x, double y, double z) {
      const int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
      const double u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
                   v = h<4 ? y : h==12 || h==14 ? x : z;
      return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
   }
};

class RegionGenerator {
  constexpr static float world_bounds = world_size * tile_size;
  PerlinNoise _noise;

public:
  RegionGenerator(int seed = 0) : _noise(seed) {}

  void generate(Region& region) {
    auto& data = region._data;

    if (data.empty() || data[0].empty()) {
      throw std::runtime_error("Bad region");
    }

    for (size_t x = 0; x < data.size(); ++x) {
      for (size_t y = 0; y < data[0].size(); ++y) {
        double f = _noise(x / 20.f, y / 20.f); 

        if (f < 0.3) {
          data[x][y] = Tile::WATER;
        }
      }
    }
  }
};
