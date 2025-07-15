#include "engine/graphics/Renderer.hpp"
#include "engine/core/GameObject.hpp"
#include "engine/graphics/Shader.hpp"
#include "engine/game/player/Camera.hpp"
#include "engine/resources/ResourceManager.hpp"
#include "engine/utils/Globals.hpp"
#include "engine/utils/errorReporting.hpp"

Renderer::Renderer() {}
Renderer::~Renderer() {}

bool Renderer::init()
{
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Falha ao inicializar GLAD");
    return false;
  }

	enableReportGlErrors();

  glEnable(GL_DEPTH_TEST);
  
  // Face Cull
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  
  // Para PNG
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glViewport(0, 0, Globals::WIDTH, Globals::HEIGHT);

	std::cout << "[OpenGL] OpenGL carregado com sucesso." << std::endl;
  return true;
}

void Renderer::render(const Scene &scene, const Camera &camera)
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  drawTerrain(scene.getTerrain(), ResourceManager::getShader("terrain"), camera);

  // apenas desenhe skybox se camera for perspectiva
  if (camera.getProjectionType() == ProjectionType::Perspective)
    drawSkybox(scene.getSkybox(), ResourceManager::getShader("skybox"), camera);
  draw(scene.getObjects(), camera);
}


void Renderer::draw(const std::vector<GameObject*> gameObjects, const Camera& camera) const
{
  for (const auto &obj: gameObjects)
  {
    // if (!obj->isActive || !obj->shader || !camera.isInFrustum(obj->transform.position, 10.0f))
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
    } 

    obj->shader->setVec3("camPos", camera.getPosition());

    obj->shader->setVec4("lightColor", glm::vec4(1.0f));
    obj->shader->setVec3("lightPos", glm::vec3(50.0f, 10.0f, 20.0f));

    obj->draw(model);
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
