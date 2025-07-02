#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include "core/GameObject.hpp"

class Scene {
public:
  void addObject(const GameObject& object);
  // TODO:
  // remover objetos
  const std::vector<GameObject>& getObjects() const;
  void update(float deltaTime);

  void init();
private:
  std::vector<GameObject> objects;
  std::vector<const GameObject*> objectPointers;
};
#endif // !SCENE_CLASS_H
