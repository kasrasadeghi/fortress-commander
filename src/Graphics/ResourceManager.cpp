#include "ResourceManager.h"
#include <string>

std::array<Shader, 1> ResourceManager::_shaders = { 
  std::string("instanced_array")
};

Shader& ResourceManager::getShader(SHADER_INDEX i) {
  Shader& s = _shaders[static_cast<std::size_t>(i)];
  if (s.ID == 0) s.load();
  return s;
}
