#include <string>
#include <fstream>
#include <unordered_map>

#include "Shader.h"
class ResourceManager {

  //TODO: consider using a enum class and an array to hold all of the shaders
  static std::unordered_map<std::string, Shader> _shader_map;

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
      
      f.seekg(0, std::ios::beg);  // go to beginning
      f.read(&contents[0], contents.size());
      f.close(); // read whole file and close
      
      return contents;
    }
    return "";
  }

  static Shader& loadShader(std::string);
};