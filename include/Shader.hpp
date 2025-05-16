#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
  // program ID
  unsigned int ID;

  // constructor
  Shader(const char* vertexPath, const char* fragmentPath);
  
  void Delete();
  void use();

  // helper function to read files and return content as c string
  static std::string get_file_contents(const std::string& path);
  
  // helpers functions for uniforms
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setVec3(const std::string& name, const glm::vec3& value) const;
  void setMat4(const std::string& name, const glm::mat4& value) const;
  void setMat3(const std::string& name, const glm::mat3& value) const;
private:
  void checkCompileErrors(unsigned int shader, std::string type);
};

#endif
