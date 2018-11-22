#include "ResourceManager.h"

static std::unordered_map<std::string, Shader> _shader_map {};

Shader& ResourceManager::loadShader(std::string name) {
  // get or construct in place from _shader_map
  auto ptr = _shader_map.find(name);
  if (ptr == _shader_map.end()) {
    return _shader_map
        .emplace(std::piecewise_construct, std::forward_as_tuple(name),
                 std::forward_as_tuple("shaders/" + name + ".vs", "shaders/" + name + ".fs"))
        .first->second;
  } else {
    return ptr->second;
  }
}
