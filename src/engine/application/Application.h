#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <exception>
#include "../data_model/Component.h"
#include "../core/Engine.h"
#include "SceneBuilder.h"

using namespace std;

class Application {
 private:
  const char* name;
  int width, height;
  unique_ptr<SceneBuilder> sceneBuilder = unique_ptr<SceneBuilder>(new SceneBuilder());

 public:
  Application(const char* name, int width, int height): name(name), width(width), height(height) {}
  void start();
  SceneBuilder* getSceneBuilder() const {
    return sceneBuilder.get();
  }
};

void resizeCallback(GLFWwindow* window, int width, int height);

#endif
