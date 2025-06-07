#include "Model.hpp"

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
  Assimp::Importer import;
  const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }
  directory = path.substr(0, path.find_last_of('/'));

  processNode(scene->mRootNode, scene);
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

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 vector;
    
    // Posições 
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    
    vertex.position = vector;

    // Normal
    if (mesh->HasNormals()) {
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.normal = vector;
    } else {
      vector.x = 0.0f;
      vector.y = 0.0f;
      vector.z = 1.0f; 
      vertex.normal = vector;
    }

    // Texturas
    if (mesh->mTextureCoords[0]) {
      glm::vec2 vec;
      // assumindo que não vão ter várias texturas pra um vértice
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.texCoords = vec;
    } else {
      vertex.texCoords = glm::vec2(1.0f, 0.0f); 
    }

    vertices.push_back(vertex);
  }
  
  // agora para vertices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];

    for (unsigned int j = 0; j < face.mNumIndices; j++) 
      indices.push_back(face.mIndices[j]);
  }
  
  // processar materiais
  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  
  // Por enquanto...
  // diffuse = diffuseN
  // specular = specularN
  
  std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

  std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

  // cria a mesh e retorna ela  
  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
  std::vector<Texture> load_textures;

  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    
    mat->GetTexture(type, i, &str);
    bool skip = false;
    for (unsigned int j = 0; j < textures_loaded.size(); j++) {
      std::string path = this->directory + '/' + str.C_Str();
      
      if (std::strcmp(textures_loaded[j].path.data(), path.c_str()) == 0) {
        // Se ja tá carregada
        load_textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }
    if (!skip) {
      // Adicionar a textura 
      std::string path = this->directory + '/' + str.C_Str();
      Texture texture(path.c_str(), typeName.c_str(), textures_loaded.size());
      load_textures.push_back(texture);
      textures_loaded.push_back(texture);
    }
  }
  return load_textures;
}
