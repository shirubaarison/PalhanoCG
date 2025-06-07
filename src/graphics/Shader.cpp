#include <graphics/Shader.hpp>
#include <fstream>
#include <iostream>

std::string Shader::get_file_contents(const std::string& path) {
  std::string result = "";
  std::ifstream file(path);

  if (!file) {
    throw std::runtime_error("Falha ao abrir arquivo: " + path);
  }

  // std::cout << "[GET_FILE_CONTENTS] arquivo " << path << " aberto com sucesso." << std::endl;

  std::string line = "";
  if (file.is_open()) {
    while (std::getline(file, line)) {
      result += line + '\n';
    }
    file.close();
  }

  return result;
}

Shader::Shader() {}

Shader::Shader(const char* vertexPath, const char* fragmentPath) 
{
  // extract data
  std::string vertexData = get_file_contents(vertexPath);
  std::string fragmentData = get_file_contents(fragmentPath);

  const char* vertexSource = vertexData.c_str();
  const char* fragmentSource = fragmentData.c_str();

  // compile shaders
  unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertex, 1, &vertexSource, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");

  unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fragment, 1, &fragmentSource, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");
  
  // shader program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  checkCompileErrors(ID, "PROGRAM");

  std::cout << "[SHADER_CLASS] Shaders " << vertexPath << " & " 
    << fragmentPath  << " compilados e linkados com sucesso." << std::endl;

  // delete
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::Delete() 
{
  glUseProgram(0);
}

void Shader::use() 
{
  glUseProgram(ID);
}

// helpers functions for uniforms
void Shader::setInt(const std::string& name, int value) const 
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const 
{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const 
{
  glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
  glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
  unsigned int size = 1024;

  int success;
  char* info = new char[size];

  if (type == "VERTEX" || type == "FRAGMENT") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
      glGetShaderInfoLog(shader, size, NULL, info);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR para: " << type << "\n" << info << std::endl;
    } 
  }
  else if (type == "PROGRAM") {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);

    if (!success) {
      glGetProgramInfoLog(shader, size, NULL, info);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_ERROR\n" << info << std::endl;
    }
  }
}

