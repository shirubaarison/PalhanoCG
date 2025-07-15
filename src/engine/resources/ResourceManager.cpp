#include "engine/resources/ResourceManager.hpp"
#include <GL/gl.h>

std::map<std::string, Model> ResourceManager::Models;
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture> ResourceManager::Textures;

Model& ResourceManager::createModel(const std::string& name)
{
  Models[name] = Model();

  return Models[name];
}

Model& ResourceManager::loadModel(const std::string& name, const std::string& path)
{
	Models[name] = Model(path);

	return Models[name];
}

Model& ResourceManager::getModel(const std::string& name)
{
	return Models[name];
}

Shader& ResourceManager::loadShader(const std::string& name, const char* vertexPath, const char* fragmentPath)
{
	Shaders[name] = Shader(vertexPath, fragmentPath);

	return Shaders[name];
}

Shader& ResourceManager::getShader(const std::string& name)
{
	return Shaders[name];
}

Texture& ResourceManager::loadTexture(const std::string& name, const char* path, GLenum wrapping, const char* texType, GLuint slot) 
{
	Textures[name] = Texture(path, texType, slot, wrapping);

	return Textures[name];
}

Texture& ResourceManager::getTexture(const std::string& name)
{
	return Textures[name];
}

void ResourceManager::clear()
{
	for (auto& [name, shader] : Shaders)
		shader.Delete();
	
	for (auto& [name, texture] : Textures)
		texture.Delete();
}
