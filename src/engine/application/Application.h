#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <exception>

class Application {
 private:
  GLFWwindow* window{};
  const char* name;
  int width, height;

 public:
  Application(const char* name, int width, int height): name(name), width(width), height(height) {}
  void start();
};

void resizeCallback(GLFWwindow* window, int width, int height);

#endif
