#ifndef RESOURCE_MANAGER_CLASS_H
#define RESOURCE_MANAGER_CLASS_H

#include <map>
#include <string>
#include <memory>

#include "Shader.hpp"
#include "Model.hpp"

class ResourceManager {
public:
  static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Model> Models;
	static std::map<std::string, Texture> Textures;

	static ResourceManager& getInstance() {
		static ResourceManager instance;
		return instance;
	}

	Shader& loadShader(const std::string& name, const char* vertexPath, const char* fragmentPath);
	Shader& getShader(const std::string& name);

	Model& loadModel(const std::string& name, const std::string& path);
	Model& getModel(const std::string& name);

	Texture& loadTexture(const std::string& name, const char* path, const char* texType, GLuint slot);
	Texture& getTexture(const std::string& name);

	void clear();
private:
	ResourceManager();
};

#endif