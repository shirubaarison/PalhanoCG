#include "core/Scene.hpp"
#include "core/Billboard.hpp"
#include "core/GameObject.hpp"
#include "resources/ResourceManager.hpp" 

Scene::Scene() : skybox(nullptr) {} // Initialize skybox to nullptr

Scene::~Scene() {
    // // CRITICAL: Manually delete all GameObjects owned by the scene
    // for (GameObject* obj : objects) {
    //     delete obj; // Delete the dynamically allocated object
    // }
    // objects.clear(); // Clear the vector of now invalid pointers
    //
    // // Manually delete skybox if it's a raw pointer.
    // delete skybox;
    // skybox = nullptr;
}

void Scene::addObject(GameObject* object)
{
    objects.push_back(object);
}

const std::vector<GameObject*>& Scene::getObjects() const
{
    return objects;
}

void Scene::update(float deltaTime) {
    for (const auto& obj_ptr : objects) { 
    if (obj_ptr && obj_ptr->isActive) {
            obj_ptr->updatePhysics(deltaTime);
        }
    }
}

const Skybox& Scene::getSkybox() const {
    if (!skybox) {
        throw std::runtime_error("Skybox not initialized in scene.");
    }
    return *skybox;
}

void Scene::init()
{
    // SKYBOX
    skybox = new Skybox("assets/skybox");

    // OBJETOS (MODELOS)
    ResourceManager& manager = ResourceManager::getInstance();

    addObject(new GameObject( // Use 'new'
        "obj1",
        &manager.getShader("default"),
        &manager.getModel("pendurador"),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(2.0f, 2.0f, 2.0f)
    ));

    addObject(new GameObject( // Use 'new'
        "obj2",
        &manager.getShader("default"),
        &manager.getModel("bike"),
        glm::vec3(2.0f, 0.0f, 5.0f),
        glm::vec3(0.0f),
        glm::vec3(2.0f, 2.0f, 2.0f)
    ));

    addObject(new GameObject( // Use 'new'
        "obj3",
        &manager.getShader("default"),
        &manager.getModel("casa1"),
        glm::vec3(10.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 180.0f, 0.0f),
        glm::vec3(0.01f, 0.01f, 0.01f)
    ));

    addObject(new GameObject( // Use 'new'
        "obj4",
        &manager.getShader("default"),
        &manager.getModel("pig"),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 180.0f, 0.0f),
        glm::vec3(1.0f)
    ));

    // Dynamically allocate with 'new'
    addObject(new Billboard( // Use 'new'
        &manager.getShader("billboard"),
        &manager.getTexture("tree"),
        glm::vec3(1.0f, 1.0f, 2.0f),
        2.0f,
        glm::vec3(1.0f),
        "TreeBillboard1"
    ));

    addObject(new Billboard(
        &manager.getShader("billboard"),
        &manager.getTexture("tree"),
        glm::vec3(5.0f, 1.0f, 8.0f),
        2.0f,
        glm::vec3(1.0f),
        "TreeBillboard2"
    ));
}
