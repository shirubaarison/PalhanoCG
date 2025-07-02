#include "core/Renderer.hpp"
#include "player/Camera.hpp"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::render(const Scene& scene, const Camera& camera) 
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  draw(scene.getObjects(), camera);
}

void Renderer::draw(const std::vector<std::shared_ptr<GameObject>>& gameObjects,
                    const Camera& camera) const
{
  for (const auto& obj : gameObjects) {
    if (!obj->isActive || !obj->shader || !obj->model) {
      continue;
    }

    obj->shader->use();

    // Camera
    obj->shader->setMat4("projection", camera.getProjectionMatrix());
    obj->shader->setMat4("view", camera.getViewMatrix());
    obj->shader->setVec3("camPos", camera.getPosition());

    // luzes
    obj->shader->setVec4("lightColor", glm::vec4(1.0f));
    obj->shader->setVec3("lightPos", glm::vec3(0.0f, 5.0f, 5.0f));

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, obj->transform.position);
    model = glm::rotate(model, glm::radians(obj->transform.rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(obj->transform.rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(obj->transform.rotation.z), glm::vec3(0, 0, 1));
    model = glm::scale(model, obj->transform.scale);

    obj->shader->setMat4("model", model);
    obj->model->draw(*obj->shader, model);
  }
}
