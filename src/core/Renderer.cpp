#include "core/Renderer.hpp"
#include "core/GameObject.hpp"
#include "graphics/Shader.hpp"
#include "player/Camera.hpp"
#include "resources/ResourceManager.hpp"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::render(const Terrain& terrain, const Scene &scene, const Camera &camera)
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  drawTerrain(terrain, ResourceManager::getInstance().getShader("terrain"), camera);
  drawSkybox(scene.getSkybox(), ResourceManager::getInstance().getShader("skybox"), camera);
  draw(scene.getObjects(), camera);
}


void Renderer::draw(const std::vector<GameObject*> gameObjects, const Camera& camera) const
{
  for (const auto &obj: gameObjects)
  {
    if (!obj->isActive || !obj->shader)
      continue;
  
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, obj->transform.position);
    model = glm::scale(model, obj->transform.scale);

    obj->shader->use();
    obj->shader->setMat4("projection", camera.getProjectionMatrix());
    obj->shader->setMat4("view", camera.getViewMatrix());
    obj->shader->setMat4("model", model);

    // 3D
    if (obj->objectType == ObjectType::MODEL && obj->model) {
      model = glm::rotate(model, glm::radians(obj->transform.rotation.x), glm::vec3(1, 0, 0));
      model = glm::rotate(model, glm::radians(obj->transform.rotation.y), glm::vec3(0, 1, 0));
      model = glm::rotate(model, glm::radians(obj->transform.rotation.z), glm::vec3(0, 0, 1));
      obj->shader->setVec3("camPos", camera.getPosition());

      obj->shader->setVec4("lightColor", glm::vec4(1.0f));
      obj->shader->setVec3("lightPos", glm::vec3(2.0f, 10.0f, 5.0f));

      obj->model->draw(*obj->shader, model);
    } 

    // 2D
    else if (obj->objectType == ObjectType::BILLBOARD) {
      obj->draw();
    }
  }
}

void Renderer::drawTerrain(const Terrain& terrain, Shader& shader, const Camera& camera)
{
  shader.use();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
  shader.setMat4("projection", camera.getProjectionMatrix());
  shader.setMat4("view", camera.getViewMatrix());
  shader.setMat4("model", model);
  
  // desabilitar o face culling pq tá bugando kkkkkkkkk
  glDisable(GL_CULL_FACE); 
  terrain.draw(shader);
  glEnable(GL_CULL_FACE); 
}

void Renderer::drawSkybox(const Skybox& skybox, Shader& shader, const Camera& camera)
{
  glDepthFunc(GL_LEQUAL); // passe no teste de depth
  
  shader.use();
  glm::mat3 skyboxView = glm::mat4(glm::mat3(camera.getViewMatrix()));
  shader.setMat4("projection", camera.getProjectionMatrix());
  shader.setMat4("view", skyboxView);
  
  skybox.render(shader);

  glDepthFunc(GL_LESS);
}
