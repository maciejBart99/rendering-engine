#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

const float ZOOM = 45.0f;

class Camera
{
 public:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  float Zoom;

  explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Zoom(ZOOM)
  {
    Position = position;
    WorldUp = up;
    setAngles(0, 0);
  }

  glm::mat4 GetViewMatrix() const
  {
    return glm::lookAt(Position, Position + Front, Up);
  }

  void setPosition(const glm::vec3& position) { Position = position; }
  void setFront(const glm::vec3& front) { Front = front; updateCameraVectors(); }
  void setAngles(float yaw, float pitch) {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Front = front;

    updateCameraVectors();
  }

  const glm::vec3& getPosition() const { return Position; }
  const glm::vec3& getFront() const { return Front; }

 private:
  void updateCameraVectors()
  {
    Front = glm::normalize(Front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
  }
};
#endif