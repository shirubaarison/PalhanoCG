#include "core/Renderer.hpp"
#include "player/Camera.hpp"
#include "resources/ResourceManager.hpp"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::render(const Scene &scene, const Camera &camera)
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  drawTerrain(scene.getTerrain(), ResourceManager::getInstance().getShader("terrain"), camera);
  draw(scene.getObjects(), camera);
}

void Renderer::draw(const std::vector<GameObject> &gameObjects, const Camera &camera) const
{
  for (const auto &obj : gameObjects)
  {
    if (!obj.isActive || !obj.shader || !obj.model)
      continue;

    obj.shader->use();

    obj.shader->setMat4("projection", camera.getProjectionMatrix());
    obj.shader->setMat4("view", camera.getViewMatrix());
    obj.shader->setVec3("camPos", camera.getPosition());

    obj.shader->setVec4("lightColor", glm::vec4(1.0f));
    obj.shader->setVec3("lightPos", glm::vec3(0.0f, 5.0f, 5.0f));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, obj.transform.position);
    model = glm::rotate(model, glm::radians(obj.transform.rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(obj.transform.rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(obj.transform.rotation.z), glm::vec3(0, 0, 1));
    model = glm::scale(model, obj.transform.scale);

    obj.shader->setMat4("model", model);
    obj.model->draw(*obj.shader, model);
  }
}

void Renderer::drawTerrain(const Terrain& terrain, Shader& shader, const Camera& camera)
{
  shader.use();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, 13.5f, 0.0f));
  shader.setMat4("projection", camera.getProjectionMatrix());
  shader.setMat4("view", camera.getViewMatrix());
  shader.setMat4("model", model);
  
  // desabilitar o face culling pq tá bugando kkkkkkkkk
  glDisable(GL_CULL_FACE); 
  terrain.draw(shader);
  glEnable(GL_CULL_FACE); 
}

void Renderer::drawSkybox(const Skybox& skybox, Shader& shader)
{
  skybox.render(shader);
}