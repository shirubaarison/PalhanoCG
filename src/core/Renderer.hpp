#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include <vector>
#include "core/GameObject.hpp"
#include "core/Scene.hpp"
#include "player/Camera.hpp"

class Renderer {
public:
  Renderer();
  ~Renderer();

  void draw(const std::vector<GameObject>& gameObjects, const Camera& camera) const;
  void drawTerrain(const Terrain& terrain, Shader& shader, const Camera& camera);
  void drawSkybox(const Skybox& skybox, Shader& shader, const Camera& camera);
  void render(const Scene& scene, const Camera& camera);
};

#endif // RENDERER_CLASS_H
