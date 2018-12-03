#include "ResourceManager.h"
#include <string>

<<<<<<< HEAD
std::array<Shader, 2> ResourceManager::_shaders = {std::string("instanced_array"),
                                                   std::string("text")};
=======
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::array<Shader, 3> ResourceManager::_shaders = {std::string("instanced_array"),
                                                   std::string("text"),
                                                   std::string("texture")};
>>>>>>> 1ad5d65... remove texture2D code from resourcemanager

Shader& ResourceManager::getShader(SHADER_INDEX i) {
  Shader& s = _shaders[static_cast<std::size_t>(i)];
  if (s.ID == 0) s.load();
  return s;
}
