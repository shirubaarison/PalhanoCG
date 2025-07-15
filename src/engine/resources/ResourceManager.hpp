#ifndef RESOURCE_MANAGER_CLASS_H
#define RESOURCE_MANAGER_CLASS_H

#include "engine/graphics/Shader.hpp"
#include "engine/graphics/Model.hpp"

#include <GL/gl.h>
#include <map>
#include <string>

class ResourceManager {
public:
  static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Model> Models;
	static std::map<std::string, Texture> Textures;

	ResourceManager() = delete;

	static Shader& loadShader(const std::string& name, const char* vertexPath, const char* fragmentPath);
	static Shader& getShader(const std::string& name);
  
  static Model& createModel(const std::string& name);
	static Model& loadModel(const std::string& name, const std::string& path);
	static Model& getModel(const std::string& name);

	static Texture& loadTexture(const std::string& name, const char* path, GLenum wrapping = GL_REPEAT, const char* texType = "diffuse", GLuint slot = 0);
	static Texture& getTexture(const std::string& name);

	void clear();
};

#endif
