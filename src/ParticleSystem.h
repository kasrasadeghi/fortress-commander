#pragma once

#include "Graphics/View.h"
#include "Particles.h"
#include <vector>
#include <functional>

template <typename T>
class ParticleSystem {
  using Callback = std::function<void(View&)>;
  std::vector<T> _particles;
  View& _view;
  Callback _beforeUpdate;
  Callback _afterUpdate;

public:
  ParticleSystem(View& view, Callback beforeUpdate, Callback afterUpdate) :
    _view(view), _beforeUpdate(beforeUpdate), _afterUpdate(afterUpdate) {
    static_assert(std::is_base_of<BaseParticle, T>::value, "T must extend BaseParticle");
  }

  void update(float dt) {
    _beforeUpdate(_view);
    for (auto it = _particles.begin(); it != _particles.end(); ) {
      T& p = *it;
      p.age += dt;
      if (p.age > p.lifespan) { 
        it = _particles.erase(it); 
        continue; 
      } else {
        ++it;
      } 

      p.vel += p.accel * dt;
      p.pos += p.vel * dt; 
      p.draw(dt);
    }
    _afterUpdate(_view);
  }

  void add(T p) {
    _particles.push_back(p);
  }
};
