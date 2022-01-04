#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <exception>
#include "../data_model/Component.h"
#include "../core/Engine.h"

using namespace std;

class Application {
 private:
  const char* name;
  int width, height;
  vector<shared_ptr<Component>> components;

 public:
  Application(const char* name, int width, int height): name(name), width(width), height(height) {}
  void addComponent(const shared_ptr<Component>& component);
  void start();
};

void resizeCallback(GLFWwindow* window, int width, int height);

#endif
