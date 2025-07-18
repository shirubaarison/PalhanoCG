#include "engine/game/player/Camera.hpp"
#include "engine/utils/Globals.hpp"

Camera::Camera(glm::vec3 position)
  : 
    position(position),
    zoom(ZOOM),
    nearPlane(NEAR_PLANE),
    farPlane(FAR_PLANE),
    front(glm::vec3(0.0f, 0.0f, -1.0f)),         // direção sempre oposta
    up(glm::vec3(0.0f, 1.0f, 0.0f)),              // y para cima
    worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    projectionType(ProjectionType::Perspective),
    yaw(-90.0f),
    pitch(0.0f)
{
  updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const 
{
    return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getRight() const
{
  return right;
}

glm::vec3 Camera::getUp() const
{
  return up;
}

ProjectionType Camera::getProjectionType() const
{
  return projectionType;
}

glm::mat4 Camera::getProjectionMatrix() const {
    if (projectionType == ProjectionType::Perspective) {
        return glm::perspective(
            glm::radians(zoom),
            static_cast<float>(Globals::WIDTH) / static_cast<float>(Globals::HEIGHT),
            nearPlane,
            farPlane
        );
    } 
    float aspect = static_cast<float>(Globals::WIDTH) / static_cast<float>(Globals::HEIGHT);
    float size = zoom;
    return glm::ortho(
        -size * aspect, size * aspect,
        -size, size,
        nearPlane, farPlane
    );
}

void Camera::processRotation(float yawOffset, float pitchOffset, bool constrainPitch) {
    yaw   += yawOffset;
    pitch += pitchOffset;

    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateFrustum() 
{
  glm::mat4 VP = getProjectionMatrix() * getViewMatrix();

  frustumPlanes[0] = glm::vec4(VP[0][3] + VP[0][0], VP[1][3] + VP[1][0], VP[2][3] + VP[2][0], VP[3][3] + VP[3][0]); // Esquerda
  frustumPlanes[1] = glm::vec4(VP[0][3] - VP[0][0], VP[1][3] - VP[1][0], VP[2][3] - VP[2][0], VP[3][3] - VP[3][0]); // Direita
  frustumPlanes[2] = glm::vec4(VP[0][3] + VP[0][1], VP[1][3] + VP[1][1], VP[2][3] + VP[2][1], VP[3][3] + VP[3][1]); // Baixo
  frustumPlanes[3] = glm::vec4(VP[0][3] - VP[0][1], VP[1][3] - VP[1][1], VP[2][3] - VP[2][1], VP[3][3] - VP[3][1]); // Topo
  frustumPlanes[4] = glm::vec4(VP[0][3] + VP[0][2], VP[1][3] + VP[1][2], VP[2][3] + VP[2][2], VP[3][3] + VP[3][2]); // Near
  frustumPlanes[5] = glm::vec4(VP[0][3] - VP[0][2], VP[1][3] - VP[1][2], VP[2][3] - VP[2][2], VP[3][3] - VP[3][2]); // Far
  
  // normalizar
  for (int i = 0; i < 6; ++i) {
    float length = glm::length(glm::vec3(frustumPlanes[i]));
    frustumPlanes[i] /= length;
  }
}

bool Camera::isInFrustum(const glm::vec3& pos, float radius) const
{
  for (int i = 0; i < 6; i++) {
    float d = glm::dot(glm::vec3(frustumPlanes[i]), pos) + frustumPlanes[i].w;
    if (d < -radius)
      return false;
  }
  return true;
}

void Camera::processZoom(float scrollOffset) {
    zoom -= scrollOffset;
    if (projectionType == ProjectionType::Orthographic) {
        zoom = glm::clamp(zoom, 1.0f, 100.0f);
    }
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}

glm::vec3 Camera::getPosition() const { return position; }

glm::vec3 Camera::getFront() const { return front; }

void Camera::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

void Camera::setProjectionType(ProjectionType type) {
    projectionType = type;
}
