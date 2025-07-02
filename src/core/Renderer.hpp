#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include <vector>
#include <memory>
#include "core/GameObject.hpp"
#include "core/Scene.hpp"
#include "player/Camera.hpp"

class Renderer {
public:
  Renderer();
  ~Renderer();

  void draw(const std::vector<std::shared_ptr<GameObject>>& gameObjects, const Camera& camera) const;
  void render(const Scene& scene, const Camera& camera);
};

#endif // RENDERER_CLASS_H
