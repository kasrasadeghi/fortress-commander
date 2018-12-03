#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

enum class SHADER_INDEX { INSTANCED_ARRAY = 0, TEXT = 1 };

#include "Shader.h"
#include "stb_image.h"

class ResourceManager {
<<<<<<< HEAD

  static std::array<Shader, 2> _shaders;

=======
  static std::array<Shader, 3> _shaders;
>>>>>>> 1ad5d65... remove texture2D code from resourcemanager
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
};
