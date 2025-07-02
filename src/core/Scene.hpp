#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include "core/GameObject.hpp"
#include "world/Terrain.hpp"
#include "world/Skybox.hpp"

class Scene {
public:
  Scene();
  void addObject(const GameObject& object);
  // TODO:
  // remover objetos
  const std::vector<GameObject>& getObjects() const;
  void update(float deltaTime);

  void init();

  const Terrain& getTerrain() const;
  const Skybox& getSkybox() const;
private:
  std::vector<GameObject> objects;

  Terrain* terrain; 
  Skybox* skybox;
};
#endif // !SCENE_CLASS_H
