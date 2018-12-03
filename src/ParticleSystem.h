#pragma once

#include "Graphics/View.h"
#include "Particles.h"
#include <vector>
#include <functional>

class ParticleSystem {
  using Callback = std::function<void(View&)>;
  std::vector<BaseParticle> _particles;
  View& _view;
  Callback _beforeUpdate;
  Callback _afterUpdate;

public:
  ParticleSystem(View& view, Callback beforeUpdate, Callback afterUpdate) :
    _view(view), _beforeUpdate(beforeUpdate), _afterUpdate(afterUpdate) {} 

  void update(float dt) {
    _beforeUpdate(_view);
    for (auto it = _particles.begin(); it != _particles.end(); ++it) {
      BaseParticle& p = *it;
      p.age += dt;
      if (p.age > p.lifespan) { _particles.erase(it); }

      p.vel += p.accel * dt;
      p.pos += p.vel * dt; 
      p.draw(dt);
    }
    _afterUpdate(_view);
  }

  void add(BaseParticle&& p) {
    _particles.push_back(p);
  }
};
