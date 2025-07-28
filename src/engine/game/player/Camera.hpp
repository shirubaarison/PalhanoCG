#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Dois tipos de camera, ortho e perpsectiva
enum class ProjectionType { Perspective, Orthographic };

// Valores padrões (Alterar far plane depois?)
constexpr float YAW         = -90.0f;
constexpr float PITCH       =  0.0f;
constexpr float ZOOM        =  45.0f; 
constexpr float NEAR_PLANE  =  0.1f;
constexpr float FAR_PLANE   =  150.0f;

class Camera {
public:
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
  
  glm::mat4 getProjectionMatrix() const;
  glm::mat4 getViewMatrix() const;
  glm::vec3 getPosition() const;
  glm::vec3 getFront() const;

  void setPosition(const glm::vec3& newPos);
  void setProjectionType(ProjectionType type);
  void setScreenSize(int width, int height);

  glm::vec3 getRight() const;
  glm::vec3 getUp() const;

  ProjectionType getProjectionType() const;

  void processRotation(float yawOffset, float pitchOffset, bool constrainPitch = true);
  
  glm::vec3 position;
  float zoom;

  float perspectiveYaw, perspectivePitch;
private:
  float nearPlane;
  float farPlane;

  // vetores fundamentais da camera
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  ProjectionType projectionType;

  float yaw; 
  float pitch;

  void updateCameraVectors();
};

#endif
