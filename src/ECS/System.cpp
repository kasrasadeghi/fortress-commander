#include "Manager.h"
#include "System.h"

namespace ECS {
System::System(Manager& manager) : _manager(manager) {}

System::~System() {}

std::size_t System::updateEntities(float dt) {
  std::size_t updatedEntities = 0;

  for (auto entity = _matchingEntities.begin(); entity != _matchingEntities.end(); ++entity) {
    updateEntity(dt, *entity); // each user class should specialize this virtual pure function
    ++updatedEntities;
  }

  return updatedEntities;
}
}
