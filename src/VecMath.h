#include "SFML/System/Vector2.hpp"
#include <cmath>

float magn(sf::Vector2f v) {
  return sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f norm(sf::Vector2f v) {
  return v / magn(v);
}