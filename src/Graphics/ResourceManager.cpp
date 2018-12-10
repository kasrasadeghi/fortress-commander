#include "ResourceManager.h"
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::array<Shader, 4> ResourceManager::_shaders = {std::string("instanced_array"),
                                                   std::string("text"), std::string("texture"),
                                                   std::string("light_mask")};

Shader& ResourceManager::getShader(SHADER_INDEX i) {
  Shader& s = _shaders[static_cast<std::size_t>(i)];
  if (s.ID == 0) s.load();
  return s;
}
