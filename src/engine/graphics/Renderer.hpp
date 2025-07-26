#ifndef RENDERER_CLASS_H
#define RENDERER_CLASS_H

#include <vector>
#include "engine/core/GameObject.hpp"
#include "engine/core/Scene.hpp"
#include "engine/game/player/Camera.hpp"
#include "engine/game/world/Terrain.hpp"

class Renderer {
public:
  Renderer();
  ~Renderer();
 
  bool init();

  void draw(const std::vector<GameObject*> gameObjects, const Camera& camera) const;
  void drawTerrain(const Terrain& terrain, Shader& shader, const Camera& camera);
  void drawSkybox(const Skybox& skybox, Shader& shader, const Camera& camera);
  void render(const Scene& scene, const Camera& camera);
};

#endif // RENDERER_CLASS_H
