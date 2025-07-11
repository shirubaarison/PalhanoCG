#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
  unsigned int ID;

  Shader();
  Shader(const char* vertexPath, const char* fragmentPath);
  
  void Delete();
  void use();

  static std::string get_file_contents(const std::string& path);
  
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setVec3(const std::string& name, const glm::vec3& value) const;
  void setVec4(const std::string& name, const glm::vec4& value) const;
  void setMat4(const std::string& name, const glm::mat4& value) const;
  void setMat3(const std::string& name, const glm::mat3& value) const;
private:
  void checkCompileErrors(unsigned int shader, std::string type);
};

#endif
