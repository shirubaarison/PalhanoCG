#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "engine/graphics/Shader.hpp"
#include "engine/graphics/Mesh.hpp"

class Model {
public:
  Model();
  Model(const std::string& path);
  void draw(Shader& shader, glm::mat4 model);

  std::vector<Texture> textures_loaded;
  std::string directory;

  glm::vec3 aabbMin = glm::vec3(FLT_MAX);
  glm::vec3 aabbMax = glm::vec3(-FLT_MAX);
  glm::vec3 aabbSize = glm::vec3(0.0f);
  glm::vec3 aabbCenter = glm::vec3(0.0f);
private:
  std::vector<Mesh> meshes;

  void loadModel(const std::string& path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
  void updateAABB();
};

#endif
