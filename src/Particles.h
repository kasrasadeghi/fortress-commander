#pragma once

#include <glm/glm.hpp>

#include "Graphics/Instanced.h"
#include "Graphics/View.h"

struct BaseParticle {
  glm::vec2 pos;
  glm::vec2 vel;
  glm::vec2 accel;
  float age, lifespan;

  virtual void draw(float dt) {}
};

struct BulletParticle : public BaseParticle {
  static LineBatch* lines;

  static void beforeUpdate(View& view) {
    if (lines == nullptr) { lines = new LineBatch(); }
    lines->instances.clear();
  }

  static void afterUpdate(View& view) {
    lines->draw(view);
  }

  BulletParticle(glm::vec2 position, glm::vec2 target, float speed) : BaseParticle() {
    pos = position;
    vel = glm::normalize(target - pos) * speed;
    lifespan = glm::distance(pos, target) / speed;
  }

  void draw(float dt) {
    float f = std::min(1.f, 3 - age / lifespan * 3.f);
    lines->add()
      .points(pos, pos - glm::normalize(vel) * 0.35f)
      .lineWidth(0.1)
      .color({1, 1, 1, f});
  }
};