#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

enum class SHADER_INDEX { INSTANCED_ARRAY = 0, TEXT = 1, TEXTURE = 2, LIGHT_MASK = 3, SIMPLE_TEXTURE = 4, FOW_MASK = 5, FOW_CLOUDS = 6 };

#include "Shader.h"
#include "Texture.h"

class ResourceManager {
  static std::array<Shader, 7> _shaders;

public:
  /// fast readfile compared with other methods
  /// from the insane coding blog
  /// http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
  static std::string readFile(std::string filename) {
    if (filename == "") return "";
    std::ifstream f(filename, std::ios::in | std::ios::binary);
    if (f) {
      std::string contents;
      f.seekg(0, std::ios::end);  // read to the end of the file
      contents.resize(f.tellg()); // get position of the last character (size of file)

      f.seekg(0, std::ios::beg); // go to beginning
      f.read(&contents[0], contents.size());
      f.close(); // read whole file and close

      return contents;
    }
    return "";
  }

  static Shader& getShader(SHADER_INDEX);

  static Texture& texture() {
    static Texture tex;
    return tex;
  }
};
