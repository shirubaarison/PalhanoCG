#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H

#include "core/GameObject.hpp"

class Scene {
public:
  void addObject(const std::shared_ptr<GameObject>& object);
  void removeObject(const std::shared_ptr<GameObject>& object);
  const std::vector<std::shared_ptr<GameObject>>& getObjects() const;
  void update(float deltaTime);

private:
  std::vector<std::shared_ptr<GameObject>> objects;
};
#endif // !SCENE_CLASS_H
