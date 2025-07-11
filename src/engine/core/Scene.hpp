#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include "engine/core/GameObject.hpp"
#include "engine/game/world/Skybox.hpp"

class Scene {
public:
  Scene();
  ~Scene(); 
  
  void addObject(GameObject* object); 

  const std::vector<GameObject*>& getObjects() const;

  void update(float deltaTime);

  void init();

  const Skybox& getSkybox() const;

private:
  std::vector<GameObject*> objects;

  Skybox* skybox; 
};
#endif // !SCENE_CLASS_H

