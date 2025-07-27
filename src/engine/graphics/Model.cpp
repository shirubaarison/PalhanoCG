#include "Model.hpp"
#include "assimp/material.h"

#include <iostream>

Model::Model() {}

Model::Model(const std::string& path) 
{
  loadModel(path);
}

void Model::draw(Shader& shader, glm::mat4 model)
{
  for (unsigned int i = 0; i < meshes.size(); i++)
    meshes[i].draw(shader, model);
}

void Model::loadModel(const std::string& path)
{
  aabbMin = glm::vec3(FLT_MAX);
  aabbMax = glm::vec3(-FLT_MAX);

  Assimp::Importer import;
  const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }
  directory = path.substr(0, path.find_last_of('/'));

  processNode(scene->mRootNode, scene);

  updateAABB();
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  bool useDiffuse = true;
  bool useSpecular = true;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 vector;
    
    // Posições 
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;

    aabbMin = glm::min(aabbMin, vector);
    aabbMax = glm::max(aabbMax, vector);
    
    vertex.position = vector;

    // Normal
    if (mesh->HasNormals()) {
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.normal = vector;
    } else {
      vertex.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    }

    // Texturas
    if (mesh->mTextureCoords[0]) {
      glm::vec2 vec;
      // assumindo que não vão ter várias texturas pra um vértice
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.texCoords = vec;
    } else {
      vertex.texCoords = glm::vec2(0.0f, 0.0f); 
    }

    vertices.push_back(vertex);
  }
  
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];

    for (unsigned int j = 0; j < face.mNumIndices; j++) 
      indices.push_back(face.mIndices[j]);
  }
  
  // processar materiais
  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  
  // Adicionar cores
  Material mat{};
  aiColor3D color;

  if (!mesh->HasVertexColors(0)) {
    mat.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    mat.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    mat.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    mat.shininess = 32.0f;
    
    if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
      mat.ambient = glm::vec3(color.r, color.g, color.b);
     
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
      mat.diffuse = glm::vec3(color.r, color.g, color.b);
      // adicionar cores nos vertices
      for (auto& v : vertices) {
        v.color = glm::vec4(color.r, color.g, color.b, 1.0f);
      }
    }

    if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
      mat.specular = glm::vec3(color.r, color.g, color.b);
    
    float shininess;
    if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
      mat.shininess = shininess;
  }
  
  // Texturas
  std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

  std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  
  // se não tiver texturas, usa cores
  if (diffuseMaps.empty())
    useDiffuse = false;
  
  if (specularMaps.empty())
    useSpecular = false;

  return Mesh(vertices, indices, textures, useDiffuse, useSpecular, mat);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
  std::vector<Texture> load_textures;

  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    
    mat->GetTexture(type, i, &str);
    bool skip = false;
    std::string path = this->directory + '/' + str.C_Str();
    
    for (unsigned int j = 0; j < textures_loaded.size(); j++) {
      if (std::strcmp(textures_loaded[j].path.data(), path.c_str()) == 0) {
        // Se ja tá carregada
        load_textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }
    
    if (!skip) {
      // Adicionar a textura 
      Texture texture(path.c_str(), typeName.c_str(), textures_loaded.size());
      load_textures.push_back(texture);
      textures_loaded.push_back(texture);
    }
  }
  return load_textures;
}

void Model::updateAABB()
{
  aabbSize = aabbMax - aabbMin;
  aabbCenter = (aabbMax + aabbMin) * 0.5f;
}
