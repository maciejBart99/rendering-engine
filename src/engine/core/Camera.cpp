#include "Camera.h"

void Camera::setAngles(float yaw, float pitch) {
  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  Front = front;

  updateCameraVectors();
}
void Camera::updateCameraVectors() {
  Front = glm::normalize(Front);
  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
}
