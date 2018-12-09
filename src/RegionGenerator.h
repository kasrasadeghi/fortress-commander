#pragma once

#include "Config.h"
#include "Region.h"

#include "GlmHashes.h"

#include <glm/gtc/matrix_transform.hpp>

#include <random>
#include <algorithm>

class PerlinNoise {
  const int _seed;
  double _frequency = 1.0;
  double _persistence = 0.5;
  double _lacunarity = 2.0;
  std::vector<int> _randomData;

  int _hash(uint x) {
    if (x >= _randomData.size()) {
      throw std::runtime_error("input to PerlinNoise::_hash out of bounds.");
  }
    return _randomData[x];
  }

  static double _fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
  static double _lerp(double t, double a, double b) { return a + t * (b - a); }
  static double _grad(int hash, double x, double y, double z) {
    const int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
    const double u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
                 v = h<4 ? y : h==12 || h==14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
  }

public:
  PerlinNoise(int seed) : _seed(seed) {
    // construct vector of [0..255]
    for (int i = 0; i < 256; ++i) {
      _randomData.push_back(i);
    }

    std::mt19937 mt(seed);
    auto randfunc = [&mt](int i){
      return std::uniform_int_distribution{0, i}(mt);
    };
    std::random_shuffle(_randomData.begin(), _randomData.end(), randfunc);

    _randomData.insert(_randomData.end(), _randomData.begin(), _randomData.end());
  }

  /**
   * @brief set frequency of the first octave
   */
  PerlinNoise& frequency(double f) {
    _frequency = f;
    return *this;
  }

  /**
   * @brief set lacunarity (specifies the frequency multipler between successive octaves)
   * @detail The effect of modifying the lacunarity is subtle; you may need to play with the lacunarity value to 
   * determine the effects. For best results, set the lacunarity to a number between 1.5 and 3.5.
   * http://libnoise.sourceforge.net/docs/classnoise_1_1module_1_1Perlin.html
   */
  PerlinNoise& lacunarity(double l) {
    _lacunarity = l;
    return *this;
  }

  /**
   * @brief set persistence (controls roughness)
   * @detail The persistence value determines how quickly the amplitudes diminish for successive octaves. The 
   * amplitude of the first octave is 1.0. The amplitude of each subsequent octave is equal to the product of the
   * previous octave's amplitude and the persistence value. So a persistence value of 0.5 sets the amplitude of the 
   * first octave to 1.0; the second, 0.5; the third, 0.25; etc.
   * http://libnoise.sourceforge.net/docs/classnoise_1_1module_1_1Perlin.html
   */
  PerlinNoise& persistence(double p) {
    _persistence = p;
    return *this;
  }

  /**
   * @brief generate noise
   * @detail adapted from https://mrl.nyu.edu/~perlin/noise/
   * 
   * @tparam octaves The number of octaves control the amount of detail of the Perlin noise. Adding more octaves 
   * increases the detail of the Perlin noise, but with the drawback of increasing the calculation time.
   * http://libnoise.sourceforge.net/docs/classnoise_1_1module_1_1Perlin.html
   * 
   * @return a value in the range [0, 1]
   */
  template <int octaves = 1>
  double generate(double x, double y = 0, double z = 0) {
    static_assert(octaves > 0);

    x *= _frequency;
    y *= _frequency;
    z *= _frequency;
    
    double currentAmp = 1;
    double result = 0;

    for (auto o = 0; o < octaves; ++o) {
      const int X = static_cast<int>(floor(x)) & 255; // FIND UNIT CUBE THAT
      const int Y = static_cast<int>(floor(y)) & 255; // CONTAINS POINT.
      const int Z = static_cast<int>(floor(z)) & 255;
      x -= floor(x);                // FIND RELATIVE X,Y,Z
      y -= floor(y);                // OF POINT IN CUBE.
      z -= floor(z);
      const double u = _fade(x);     // COMPUTE FADE CURVES
      const double v = _fade(y);     // FOR EACH OF X,Y,Z.
      const double w = _fade(z);

      const int A = _hash(X  )+Y, AA = _hash(A)+Z, AB = _hash(A+1)+Z, // HASH COORDINATES OF
                B = _hash(X+1)+Y, BA = _hash(B)+Z, BB = _hash(B+1)+Z; // THE 8 CUBE CORNERS,

      double noise = _lerp(w, _lerp(v, _lerp(u, _grad(_hash(AA  ), x  , y  , z   ),  // AND ADD
                                                _grad(_hash(BA  ), x-1, y  , z   )), // BLENDED
                                       _lerp(u, _grad(_hash(AB  ), x  , y-1, z   ),  // RESULTS
                                                _grad(_hash(BB  ), x-1, y-1, z   ))),// FROM  8
                              _lerp(v, _lerp(u, _grad(_hash(AA+1), x  , y  , z-1 ),  // CORNERS
                                                _grad(_hash(BA+1), x-1, y  , z-1 )), // OF CUBE
                                       _lerp(u, _grad(_hash(AB+1), x  , y-1, z-1 ),
                                                _grad(_hash(BB+1), x-1, y-1, z-1 ))));
      result += noise * currentAmp;

      // update parameters for next octave
      currentAmp *= currentAmp * _persistence;
      x *= _lacunarity;
      y *= _lacunarity;
      z *= _lacunarity;
    }

    // map from [-sqrt(n)/2, sqrt(n)/2] to [0, 1] where n is dimensionality
    result = result / sqrt(3) + 0.5f;
    return std::max(0., std::min(1., result)); // clamp because there is no guarantee on output range
  }
};

class RegionGenerator {
  constexpr static float world_bounds = world_size * tile_size;
  PerlinNoise _noise;

public:
  RegionGenerator(int seed = 80085) : _noise(seed) {
    _noise.frequency(1/30.).lacunarity(2.5);
  }

  void generate(Region& region) {
    auto& data = region._data;

    if (data.empty() || data[0].empty()) {
      throw std::runtime_error("Bad region");
    }

    for (size_t x = 0; x < data.size(); ++x) {
      for (size_t y = 0; y < data[0].size(); ++y) {
        double f = _noise.generate<3>(x, y); 

        if (f < 0.3) {
          data[x][y] = Tile::WATER;
        }
      }
    }
  }
};
